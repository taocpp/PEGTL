// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
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

#include <iostream>
#include <string>
#include <vector>

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE::example
{
   void header( const std::vector< std::string >& v )
   {
      std::cout << "|  |";
      for( const auto& s : v ) {
         std::cout << " \"" << s << "\" |";
      }
      std::cout << "\n|--|";
      for( const auto& s : v ) {
         (void)s;
         std::cout << "--|";
      }
      std::cout << "\n";
   }

   template< typename Rule >
   void row( const std::string& r, const std::vector< std::string >& v )
   {
      std::cout << "| `" << r << "` |";
      for( const auto& s : v ) {
         view_input<> in( s );
         const char* a = in.current();
         try {
            if( parse< Rule >( in ) ) {
               const std::string t( a, in.current() - a );
               std::cout << " \"" << t << "\" |";
            }
            else {
               std::cout << " f |";
            }
         }
         catch( ... ) {
            std::cout << " E |";
         }
      }
      std::cout << "\n";
   }

   using a = one< 'a' >;
   using b = one< 'b' >;
   using c = one< 'c' >;

   void table1()
   {
      std::cout << "## Simple Combinators\n\n";
      {
         const std::vector< std::string > v = { "", "a", "ab", "z", "az" };
         header( v );
         row< seq< a, b > >( "seq< a, b >", v );
         row< opt< a, b > >( "opt< a, b >", v );
         row< strict< a, b > >( "strict< a, b >", v );
         row< partial< a, b > >( "partial< a, b >", v );
         row< must< a, b > >( "must< a, b >", v );
         row< if_must< a, b > >( "if_must< a, b >", v );
      }
      std::cout << std::endl;
   }

   void table2()
   {
      std::cout << "## Iterating Combinators\n\n";
      {
         const std::vector< std::string > v = { "", "a", "ab", "aba", "abab", "z", "az", "abz", "abaz" };
         header( v );
         row< plus< a, b > >( "plus< a, b >", v );
         row< star< a, b > >( "star< a, b >", v );
         row< star_strict< a, b > >( "star_strict< a, b >", v );
         row< star_partial< a, b > >( "star_partial< a, b >", v );
         row< star_must< a, b > >( "star_must< a, b >", v );
      }
      std::cout << std::endl;
   }

   void table3()
   {
      std::cout << "## Repeating Combinators\n\n";
      {
         const std::vector< std::string > v = { "", "a", "aa", "aaa", "aaaa" };
         header( v );
         row< rep< 2, a > >( "rep< 2, a >", v );
         row< rep_opt< 2, a > >( "rep_opt< 2, a >", v );
         row< rep_min< 2, a > >( "rep_min< 2, a >", v );
         row< rep_max< 2, a > >( "rep_max< 2, a >", v );
         row< rep_min_max< 2, 3, a > >( "rep_min_max< 2, 3, a >", v );
      }
      std::cout << std::endl;
   }

   void table4()
   {
      std::cout << "## Rule List Combinators\n\n";
      {
         const std::vector< std::string > v = { "a", "aa", "ab", "aba", "abab", "abc", "ac", "acb", "acba", "acbca" };
         header( v );
         row< list< a, b > >( "list< a, b >", v );
         row< list_tail< a, b > >( "list_tail< a, b >", v );
         row< list_must< a, b > >( "list_must< a, b >", v );
         row< list< a, b, c > >( "list< a, b, c >", v );
         row< list_tail< a, b, c > >( "list_tail< a, b, c >", v );
         row< list_must< a, b, c > >( "list_must< a, b, c >", v );
      }
      std::cout << std::endl;
   }

   void behaviour()
   {
      table1();
      table2();
      table3();
      table4();
   }

}  // namespace TAO_PEGTL_NAMESPACE::example

int main()
{
   TAO_PEGTL_NAMESPACE::example::behaviour();
   return 0;
}

#endif
