// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <pegtl/contrib/string.hh>

#include <type_traits>

int main()
{
   // The strings have a maximum length of 100 characters

   static_assert( std::is_same< PEGTL_STRING( "Hello" ), pegtl::string< 'H', 'e', 'l', 'l', 'o' > >::value, "Oops" );
   static_assert( !std::is_same< PEGTL_ISTRING( "Hello" ), pegtl::string< 'H', 'e', 'l', 'l', 'o' > >::value, "Oops" );
   static_assert( std::is_same< PEGTL_ISTRING( "Hello" ), pegtl::istring< 'H', 'e', 'l', 'l', 'o' > >::value, "Oops" );
}
