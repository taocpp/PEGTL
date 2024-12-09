// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/integer.hpp>

namespace coverage
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   std::ostream& operator<<( std::ostream& os, const std::optional< std::size_t >& opt )
   {
      if( opt.has_value() ) {
         return os << *opt;
      }
      else {
         return os << "nullopt";
      }
   }

   [[nodiscard]] bool is_dummy( const std::string& source )
   {
      if( source.empty() ) {
         return true;
      }
      const auto p = source.find_first_not_of( ' ' );

      if( p == std::string::npos ) {
         return true;
      }
      if( p + 1 == source.size() ) {
         if( source[ p ] == '#' ) {
            return true;
         }
         if( source[ p ] == '{' ) {
            return true;
         }
         if( source[ p ] == '}' ) {
            return true;
         }
      }
      if( ( source[ p ] == '/' ) && ( source.size() > ( p + 1 ) ) && source[ p + 1 ] == '/' ) {
         return true;
      }
      return false;
   }

   namespace rules
   {
      struct ws : pegtl::star< pegtl::blank > {};
      struct sep : pegtl::one< '|' > {};
      struct number : pegtl::plus< pegtl::digit > {};
      struct count : pegtl::star< pegtl::digit > {};
      struct kilo : pegtl::one< 'k' > {};
      struct addon : pegtl::seq< pegtl::opt< pegtl::one< '.' >, pegtl::plus< pegtl::digit > >, pegtl::opt< kilo > > {};
      struct code : pegtl::until< pegtl::at< pegtl::eol > > {};
      struct source : pegtl::if_must< number, sep, ws, count, addon, sep, code, pegtl::eol > {};
      struct separator : pegtl::if_must< pegtl::plus< pegtl::one< '-' > >, pegtl::eol > {};
      struct specialization : pegtl::if_must< sep, pegtl::until< pegtl::eol > > {};
      struct line : pegtl::sor< source, separator, specialization > {};
      struct body : pegtl::plus< ws, line > {};
      struct name : pegtl::until< pegtl::at< pegtl::one< ':' > > > {};
      struct head : pegtl::seq< name, pegtl::any, pegtl::eol > {};
      struct file : pegtl::seq< head, body, pegtl::eol > {};
      struct main : pegtl::star< file > {};
      struct wrap : pegtl::sor< pegtl::eof, pegtl::one< ' ' >, pegtl::digit, pegtl::must< main, pegtl::eof > > {};

   }  // namespace rules

   struct per_line_data
   {
      per_line_data( std::string&& s, const std::optional< std::size_t >& c )
         : source( std::move( s ) ),
           count( c )
      {}

      std::string source;
      std::optional< std::size_t > count;
   };

   struct per_file_data
   {
      std::size_t only_zero_count = 0;
      std::vector< per_line_data > lines;
   };

   struct state
   {
      state()
      {
         lines.reserve( 1000 );
      }

      // Per line temporary state.
      std::size_t number;
      std::optional< std::size_t > count;
      // Per file temporary state.
      std::string name;
      std::vector< per_line_data > lines;
      std::size_t non_zero_count;
      // Accumulated result state.
      std::map< std::string, per_file_data > data;
   };

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct action< rules::name >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state& st )
      {
         // std::cout << "Counting " << in.string_view() << std::endl;
         st.name = in.string();
         st.lines.clear();
         st.non_zero_count = 0;
      }
   };

   template<>
   struct action< rules::number >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state& st )
      {
         assert( !in.empty() );
         std::size_t t = 0;
         const bool b = pegtl::internal::convert_unsigned( t, in.string_view() );
         (void)b;
         assert( b );
         assert( t > 0 );
         st.number = t - 1;
         st.count = std::nullopt;
      }
   };

   template<>
   struct action< rules::count >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state& st )
      {
         if( !in.empty() ) {
            std::size_t t = 0;
            const bool b = pegtl::internal::convert_unsigned( t, in.string_view() );
            (void)b;
            assert( b );
            st.count = t;
            st.non_zero_count += std::size_t( t > 0 );
         }
      }
   };

   template<>
   struct action< rules::kilo >
   {
      static void apply0( state& st )
      {
         assert( st.count );
         st.count.value() *= 1000;  // Overflow?
      }
   };

   template<>
   struct action< rules::code >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& in, state& st )
      {
         assert( st.lines.size() == st.number );
         st.lines.emplace_back( in.string(), st.count );
      }
   };

   void merge( std::optional< std::size_t >& d, const std::optional< std::size_t >& s )
   {
      if( !d.has_value() ) {
         d = s;
      }
      else if( s.has_value() ) {
         d.value() += s.value();
      }
   }

   template<>
   struct action< rules::body >
   {
      static void apply0( state& st )
      {
         per_file_data& file = st.data[ st.name ];

         if( st.non_zero_count == 0 ) {
            file.only_zero_count++;
            return;
         }
         if( file.lines.empty() ) {
            file.lines = st.lines;
            return;
         }
         assert( st.lines.size() == file.lines.size() );

         for( std::size_t i = 0; i < st.lines.size(); ++i ) {
            assert( st.lines[ i ].source == file.lines[ i ].source );
            merge( file.lines[ i ].count, st.lines[ i ].count );
         }
      }
   };

   class coverage
   {
   public:
      explicit coverage( const std::vector< std::string >& files )
      {
         for( const auto& file : files ) {
            accumulate( file );
         }
         std::cout << "Completed " << files.size() << std::endl;
      }

      void report()
      {
         std::cout << "Covered " << m_state.data.size() << std::endl;

         for( const auto& pair : m_state.data ) {
            if( pair.first.find( "include/tao/pegtl" ) != std::string_view::npos ) {
               m_current_problems = 0;
               report( pair.first, pair.second );
            }
         }
         const double fraction = double( m_total_lines - m_problem_lines ) / double( m_total_lines );
         std::cout << "Summary for lines -- total " << m_total_lines << " problems " << m_problem_lines << " coverage " << fraction << std::endl;
         std::cout << "Summary for files -- total " << m_total_files << " problems " << m_problem_files << std::endl;
      }

      void report( const std::string& name, const per_file_data& data )
      {
         m_exclude_active = false;

         std::cout << "Reporting " << name << " (" << data.only_zero_count << ")" << std::endl;

         for( const auto& line : data.lines ) {
            report( line );
         }
         if( m_current_problems > 0 ) {
            ++m_problem_files;
         }
         ++m_total_files;
      }

      void report( const per_line_data& data )
      {
         if( data.source.find( "// LCOV_EXCL_START" ) != std::string::npos ) {
            assert( !m_exclude_active );  // TODO: Don't crash on "wrong" input.
            m_exclude_active = true;
         }
         if( ( !data.count ) || is_dummy( data.source ) ) {
            std::cout << "           " << data.source << std::endl;
         }
         else if( data.count.value() == 0 ) {
            ++m_total_lines;
            if( m_exclude_active || ( data.source.find( "// LCOV_EXCL_LINE" ) != std::string::npos ) ) {
               std::cout << "         0 " << data.source << std::endl;
            }
            else {
               ++m_problem_lines;
               ++m_current_problems;
               std::cout << "000        " << data.source << std::endl;
            }
         }
         else {
            ++m_total_lines;
            std::cout << std::setw( 10 ) << data.count.value() << " " << data.source << std::endl;
         }
         if( data.source.find( "// LCOV_EXCL_STOP" ) != std::string::npos ) {
            assert( m_exclude_active );  // TODO: Don't crash on "wrong" input.
            m_exclude_active = false;
         }
      }

      void accumulate( const std::string& file )
      {
         pegtl::text_mmap_input<> in( file );
         // std::cout << "Parsing " << file << std::endl;
         pegtl::parse< pegtl::must< rules::wrap >, action >( in, m_state );
      }

   private:
      state m_state;

      std::size_t m_total_lines = 0;
      std::size_t m_problem_lines = 0;

      std::size_t m_total_files = 0;
      std::size_t m_problem_files = 0;

      std::size_t m_current_problems = 0;
      bool m_exclude_active = false;
   };

}  // namespace coverage

int main( int argc, char** argv )
{
   coverage::coverage( std::vector< std::string >( argv + 1, argv + argc ) ).report();
   return 0;
}
