// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

namespace csv1
{
   // Simple CSV-file format for an unknown-at-compile-time number of values per
   // line, the values are space/tab-padded integers, comment lines start with
   // a hash and are ignored; neither the grammar nor the included actions make
   // sure that the number of values per line is always the same; last line can
   // end with an LF or CR+LF but doesn't have to.

   // Example file contents parsed by this grammar (excluding C++ comment intro):
   // # This is a comment
   // 123 , 124,41,1
   //  1,2,3,4
   // 1
   //    1,2

   // clang-format off
   struct value : tao::TAOCPP_PEGTL_NAMESPACE::plus< tao::TAOCPP_PEGTL_NAMESPACE::digit > {};
   struct value_item : tao::TAOCPP_PEGTL_NAMESPACE::pad< value, tao::TAOCPP_PEGTL_NAMESPACE::blank > {};
   struct value_list : tao::TAOCPP_PEGTL_NAMESPACE::list_must< value_item, tao::TAOCPP_PEGTL_NAMESPACE::one< ',' > > {};
   struct value_line : tao::TAOCPP_PEGTL_NAMESPACE::if_must< value_list, tao::TAOCPP_PEGTL_NAMESPACE::eolf > {};
   struct comment_line : tao::TAOCPP_PEGTL_NAMESPACE::seq< tao::TAOCPP_PEGTL_NAMESPACE::one< '#' >, tao::TAOCPP_PEGTL_NAMESPACE::until< tao::TAOCPP_PEGTL_NAMESPACE::eolf > > {};
   struct line : tao::TAOCPP_PEGTL_NAMESPACE::sor< comment_line, value_line > {};
   struct file : tao::TAOCPP_PEGTL_NAMESPACE::until< tao::TAOCPP_PEGTL_NAMESPACE::eof, line > {};
   // clang-format on

   // Data structure to store the result of a parsing run:

   using result_data = std::vector< std::vector< unsigned long > >;

   // Action and control classes to fill in the above data structure:

   template< typename Rule >
   struct action
      : tao::TAOCPP_PEGTL_NAMESPACE::nothing< Rule >
   {
   };

   template<>
   struct action< value >
   {
      template< typename Input >
      static void apply( const Input& in, result_data& data )
      {
         assert( !data.empty() );
         data.back().push_back( std::stoul( in.string() ) );
      }
   };

   template< typename Rule >
   struct control
      : tao::TAOCPP_PEGTL_NAMESPACE::normal< Rule >
   {
   };

   template<>
   struct control< value_line >
      : tao::TAOCPP_PEGTL_NAMESPACE::normal< value_line >
   {
      template< typename Input >
      static void start( Input& /*unused*/, result_data& data )
      {
         data.emplace_back();
      }

      template< typename Input >
      static void failure( Input& /*unused*/, result_data& data )
      {
         assert( !data.empty() );
         data.pop_back();
      }
   };

}  // csv1

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      tao::TAOCPP_PEGTL_NAMESPACE::file_input<> in( argv[ i ] );
      csv1::result_data data;
      tao::TAOCPP_PEGTL_NAMESPACE::parse< tao::TAOCPP_PEGTL_NAMESPACE::must< csv1::file >, csv1::action, csv1::control >( in, data );
      for( const auto& line : data ) {
         assert( !line.empty() );  // The grammar doesn't allow empty lines.
         std::cout << line.front();
         for( std::size_t j = 1; j < line.size(); ++j ) {
            std::cout << ", " << line[ j ];
         }
         std::cout << std::endl;
      }
   }
   return 0;
}
