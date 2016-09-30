// Copyright (c) 2015-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <type_traits>

#include <pegtl.hh>
#include <pegtl/contrib/alphabet.hh>

namespace test
{
   // We only need to test that this compiles...

   struct foo : pegtl_string_t( "foo" ) {};
   struct foobar : pegtl::sor< pegtl_string_t( "foo" ), pegtl_string_t( "bar" ) > {};

   static_assert( std::is_same< pegtl_string_t( "Hello" ), pegtl::string< 'H', 'e', 'l', 'l', 'o' > >::value, "pegtl_string_t broken" );
   static_assert( ! std::is_same< pegtl_istring_t( "Hello" ), pegtl::string< 'H', 'e', 'l', 'l', 'o' > >::value, "pegtl_istring_t broken" );
   static_assert( std::is_same< pegtl_istring_t( "Hello" ), pegtl::istring< 'H', 'e', 'l', 'l', 'o' > >::value, "pegtl_istring_t broken" );

   // Strings may even contain embedded nulls

   static_assert( std::is_same< pegtl_string_t( "Hello, w\0rld!" ), pegtl::string< 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 0, 'r', 'l', 'd', '!' > >::value, "pegtl_string_t broken" );

   // The strings currently have a maximum length of 512 characters.

   using namespace pegtl::alphabet;
   static_assert( std::is_same< pegtl_string_t( "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" ),
                  pegtl::string< a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z > >::value, "pegtl_string_t broken" );

} // namespace test

int main()
{
   return 0;
}
