// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP
#define TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP

#include <cassert>
#include <cstdio>
#include <ostream>
#include <string_view>

#include "../config.hpp"
#include "../demangle.hpp"
#include "../rules.hpp"

#include "../internal/peek_char.hpp"
#include "../internal/peek_utf8.hpp"

#include "forward.hpp"

#include "internal/print_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template<>
   struct print_rules_traits< void >
   {
      static constexpr std::string_view literal_style = "\033[38;5;2m";
      static constexpr std::string_view pegtl_style = "\033[38;5;238m";  // "\033[90m";
      static constexpr std::string_view quote_style = "\033[1m";
      static constexpr std::string_view user_style = "\033[38;5;18m";  // "\033[34m";

      print_rules_traits() = default;

      explicit print_rules_traits( const std::string_view in_include )
         : include( in_include )
      {}

      static internal::styled literal( const std::string_view string ) noexcept
      {
         return { literal_style, string };
      }

      static internal::styled pegtl( const std::string_view string ) noexcept
      {
         return { pegtl_style, string };
      }

      static internal::styled user( const std::string_view string ) noexcept
      {
         return { user_style, string };
      }

      const std::string_view include;
      const std::string_view exclude = "tao::pegtl::";

      template< typename Rule >
      [[nodiscard]] std::string_view name() const noexcept
      {
         const std::string_view d = demangle< Rule >();
         if( ( !is_in_namespace( d, include ) ) && is_in_namespace( d, exclude ) ) {
            return std::string_view();
         }
         const std::string_view::size_type b = d.find( '<' );
         const std::string_view::size_type c = d.rfind( ':', b );
         return ( c == std::string_view::npos ) ? d : d.substr( c + 1 );
      }

      template< template< typename... > class Traits, typename Rule >
      void print_rule( std::ostream& os ) const
      {
         if( const auto rule = Traits< void >::template name< Rule >(); !rule.empty() ) {
            os << user( rule );
         }
         else {
            Traits< typename Rule::rule_t >::template print< Traits >( os, *this );
         }
      }

      template< template< typename... > class Traits, typename Rule, typename... Rules >
      void print_list( std::ostream& os, const char* a, const char* b = "( ", [[maybe_unused]] const char* c = ", ", const char* d = " )" ) const
      {
         if( a ) {
            os << pegtl( a );
         }
         os << b;
         ( print_rule< Traits, Rule >( os ), ..., ( os << c, print_rule< Traits, Rules >( os ) ) );
         os << d;
      }

      void escape( std::ostream& os, const char32_t i ) const
      {
         switch( i ) {
            case '"':
               os << "\\\"";
               return;
            case '\\':
               os << "\\\\";
               return;
            case '\a':
               os << "\\a";
               return;
            case '\b':
               os << "\\b";
               return;
            case '\t':
               os << "\\t";
               return;
            case '\n':
               os << "\\n";
               return;
            case '\r':
               os << "\\r";
               return;
            case '\v':
               os << "\\v";
               return;
            default:
               break;
         }
         if( ( 32 <= i ) && ( i <= 126 ) ) {
            os << char( i );
            return;
         }
         if( i < 0x10000 ) {
            char b[ 8 ];
            const auto s = std::snprintf( b, sizeof( b ), "\\u%04x", unsigned( i ) );
            os.write( b, s );
            return;
         }
         if( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "\\U%06x", unsigned( i ) );
            os.write( b, s );
            return;
         }
         assert( false );  // Or what?
      }

      void escape1( std::ostream& os, const char32_t i ) const
      {
         switch( i ) {
            case '\'':
               os << "'\\''";
               return;
            case '\\':
               os << "'\\\\'";
               return;
            case '\a':
               os << "'\\a'";
               return;
            case '\b':
               os << "'\\b'";
               return;
            case '\t':
               os << "'\\t'";
               return;
            case '\n':
               os << "'\\n'";
               return;
            case '\r':
               os << "'\\r'";
               return;
            case '\v':
               os << "'\\v'";
               return;
            default:
               break;
         }
         if( ( 32 <= i ) && ( i <= 126 ) ) {
            os << '\'' << char( i ) << '\'';
            return;
         }
         if( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "U+%X", unsigned( i ) );
            os.write( b, s );
            return;
         }
         assert( false );  // Or what?
      }

   private:
      [[nodiscard]] static bool is_in_namespace( const std::string_view rule_name, const std::string_view name_space ) noexcept
      {
         // TODO: Check whether this needs tweaking for Windows and/or MSVC.
         return ( rule_name.size() > name_space.size() ) && ( rule_name.compare( 0, name_space.size(), name_space ) == 0 );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "alnum" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "alpha" );
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_char > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "any" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::at< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "at" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::at< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "at" );
      }
   };

   template<>
   struct print_rules_traits< internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\t' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "blank" );
      }
   };

   template<>
   struct print_rules_traits< internal::bof >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "bof" );
      }
   };

   template<>
   struct print_rules_traits< internal::bol >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "bol" );
      }
   };

   template< typename Peek >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, char32_t( 0xfeff ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "bom" );
      }
   };

   template< unsigned Num >
   struct print_rules_traits< internal::bytes< Num > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "bytes" ) << "( " << Num << " )";
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "digit" );
      }
   };

   template<>
   struct print_rules_traits< internal::string< '.', '.', '.' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "ellipsis" );
      }
   };

   template<>
   struct print_rules_traits< internal::eof >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "eof" );
      }
   };

   template<>
   struct print_rules_traits< internal::eol >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "eol" );
      }
   };

   template<>
   struct print_rules_traits< internal::eolf >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "eolf" );
      }
   };

   template<>
   struct print_rules_traits< internal::failure >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "failure" );
      }
   };

   template< char C, char... Cs >
   struct print_rules_traits< internal::rep< 42, internal::one< internal::result_on_found::success, internal::peek_char, C, Cs... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "forty_two" ) << "( ";
         ( pc.escape1( os, C ), ..., ( os << ", ", pc.escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template<>
   struct print_rules_traits< internal::seq< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '_' >, internal::star< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "identifier" );
      }
   };

   // TODO: identifier_first?
   // TODO: identifier_other?

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< false, Cond, Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rules... >( os, "if_must" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, Then, Else > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Then, Else >( os, "if_then_else" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, internal::must< Then >, internal::must< Else > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Then, Else >( os, "if_must_else" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::istring< Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "istring" ) << "( \"";
         ( pc.escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::seq< internal::string< Cs... >, internal::not_at< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "keyword" ) << "( \"";
         ( pc.escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, Rule > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep >( os, "list" );
      }
   };

   template< typename Rule, typename Sep, typename Pad >
   struct print_rules_traits< internal::seq< Rule, internal::star< internal::seq< internal::star< Pad >, Sep, internal::star< Pad > >, Rule > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep, Pad >( os, "list" );
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, internal::must< Rule > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep >( os, "list_must" );
      }
   };

   template< typename Rule, typename Sep, typename Pad >
   struct print_rules_traits< internal::seq< Rule, internal::star< internal::seq< internal::star< Pad >, Sep, internal::star< Pad > >, internal::must< Rule > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep, Pad >( os, "list_must" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "lower" );
      }
   };

   template< typename Head, typename Rule >
   struct print_rules_traits< internal::rematch< Head, internal::not_at< Rule, internal::eof > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Head, Rule >( os, "minus" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::must< Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "must" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::not_at< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "not_at" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::not_at< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "not_at" );
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::failure, Peek, C, Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "not_one" ) << "( ";
         ( pc.escape1( os, C ), ..., ( os << ", ", pc.escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::failure, Peek, Lo, Hi > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "not_range" ) << "( ";
         pc.escape1( os, Lo );
         os << " - ";
         pc.escape1( os, Hi );
         os << " )";
      }
   };

   template< typename Peek >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, typename Peek::data_t( 0 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "nul" );
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, C, Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "one" ) << "( ";
         ( pc.escape1( os, C ), ..., ( os << ", ", pc.escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::opt< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "opt" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::opt< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "opt" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< true, Cond, Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rules... >( os, "opt_must" );
      }
   };

   template< typename Rule, typename Pad >
   struct print_rules_traits< internal::seq< internal::star< Pad >, Rule, internal::star< Pad > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad >( os, "pad" );
      }
   };

   template< typename Rule, typename Pad1, typename Pad2 >
   struct print_rules_traits< internal::seq< internal::star< Pad1 >, Rule, internal::star< Pad2 > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad1, Pad2 >( os, "pad" );
      }
   };

   template< typename Rule, typename Pad >
   struct print_rules_traits< internal::seq< internal::star< Pad >, internal::opt< Rule, internal::star< Pad > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad >( os, "pad_opt" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::plus< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "plus" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::plus< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "plus" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, char( 32 ), char( 126 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "print" );
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::success, Peek, Lo, Hi > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "range" ) << "( ";
         pc.escape1( os, Lo );
         os << " - ";
         pc.escape1( os, Hi );
         os << " )";
      }
   };

   namespace internal
   {
      template< template< typename... > class Traits, typename T, T... >
      struct print_ranges_rules_traits;

      template< template< typename... > class Traits, typename T, T C >
      struct print_ranges_rules_traits< Traits, T, C >
      {
         template< typename Config >
         static void print( std::ostream& os, const Config& pc )
         {
            os << ", ";
            pc.escape1( os, C );
         }
      };

      template< template< typename... > class Traits, typename T, T Lo, T Hi, T... Ts >
      struct print_ranges_rules_traits< Traits, T, Lo, Hi, Ts... >
      {
         template< typename Config >
         static void print( std::ostream& os, const Config& pc )
         {
            os << ", ";
            pc.escape1( os, Lo );
            os << " - ";
            pc.escape1( os, Hi );
         }
      };

   }  // namespace internal

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::ranges< Peek, Lo, Hi, Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "ranges" ) << "( ";
         pc.escape1( os, Lo );
         os << " - ";
         pc.escape1( os, Hi );
         internal::print_ranges_rules_traits< Traits, typename Peek::data_t, Cs... >::print( os, pc );
         os << " )";
      }
   };

   template< typename Head, typename... Rules >
   struct print_rules_traits< internal::rematch< Head, Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Head, Rules... >( os, "rematch" );
      }
   };

   template< unsigned Cnt, typename Rule >
   struct print_rules_traits< internal::rep< Cnt, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep" ) << "( " << Cnt;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep< Cnt, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep" ) << "( " << Cnt;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Min, unsigned Max, typename Rule >
   struct print_rules_traits< internal::rep_min_max< Min, Max, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_min_max" ) << "( " << Min << ", " << Max;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Min, unsigned Max, typename... Rules >
   struct print_rules_traits< internal::rep_min_max< Min, Max, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_min_max" ) << "( " << Min << ", " << Max;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename Rule >
   struct print_rules_traits< internal::rep_opt< Cnt, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_opt" ) << "( " << Cnt;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep_opt< Cnt, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_opt" ) << "( " << Cnt;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::seq< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::seq< Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "seq" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, char( 0 ), char( 127 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "seven" );
      }
   };

   template<>
   struct print_rules_traits< internal::seq< internal::string< '#', '!' >, internal::until< internal::eolf > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "shebang" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::sor< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::sor< Rules... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "sor" );
      }
   };

   template<>
   struct print_rules_traits< internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\n', '\r', '\t', '\v', '\f' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "space" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::star< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "star" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::star< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "star" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::star< internal::if_must< false, Cond, Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "star_must" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::string< Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "string" ) << "( \"";
         ( pc.escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template<>
   struct print_rules_traits< internal::success >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "success" );
      }
   };

   template< char C >
   struct print_rules_traits< internal::string< C, C, C > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "three" ) << "( ";
         pc.escape1( os, C );
         os << " )";
      }
   };

   template< char C >
   struct print_rules_traits< internal::string< C, C > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "two" ) << "( ";
         pc.escape1( os, C );
         os << " )";
      }
   };

   template< typename Cond >
   struct print_rules_traits< internal::until< Cond > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond >( os, "until" );
      }
   };

   template< typename Cond, typename Rule >
   struct print_rules_traits< internal::until< Cond, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rule >( os, "until" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::until< Cond, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rules... >( os, "until" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "upper" );
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_utf8 > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "utf8" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "xdigit" );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
