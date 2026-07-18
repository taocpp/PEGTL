// Copyright (c) 2016-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

#include <tao/pegtl.hpp>
#include <tao/pegtl/action/builders.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

namespace example
{
   // Simple CSV-file format for an unknown-at-compile-time number of values per
   // line, the values are space/tab-padded integers, comment lines start with
   // a hash and are ignored; the grammar does not require the same number of
   // values on every line, and the last line need not end with an eol.

   // clang-format off
   struct value : pegtl::plus< pegtl::digit > {};
   struct value_item : pegtl::pad< value, pegtl::blank > {};
   struct value_list : pegtl::list< value_item, pegtl::one< ',' > > {};
   struct value_line : pegtl::seq< value_list, pegtl::eolf > {};
   struct comment_line : pegtl::seq< pegtl::one< '#' >, pegtl::until< pegtl::eolf > > {};
   struct line : pegtl::sor< comment_line, value_line > {};
   struct file : pegtl::until< pegtl::eof, line > {};
   // clang-format on

   using row = std::vector< unsigned long >;

   struct result_data
   {
      std::vector< row > rows;
   };

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct action< file >
      : pegtl::repeat_to< &result_data::rows,
                          pegtl::repeat_for< value_line, value > >
   {};

}  // namespace example

int main( int argc, char** argv )  // NOLINT(bugprone-exception-escape)
{
   for( int i = 1; i < argc; ++i ) {
      pegtl::text_file_input< pegtl::lazy::lf_crlf > in( argv[ i ] );
      example::result_data data;
      if( !pegtl::parse< pegtl::seq< example::file >, example::action >( in, data ) ) {
         std::cerr << "Parse error!" << std::endl;
         return 1;
      }
      for( const auto& row : data.rows ) {
         assert( !row.empty() );  // The grammar doesn't allow empty value lines.
         std::cout << row.front();
         for( std::size_t j = 1; j < row.size(); ++j ) {
            std::cout << ", " << row[ j ];
         }
         std::cout << std::endl;
      }
   }
   return 0;
}

#endif
