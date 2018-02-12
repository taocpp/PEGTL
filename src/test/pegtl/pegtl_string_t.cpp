// Copyright (c) 2015-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <type_traits>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/alphabet.hpp>

namespace test
{
   // We only need to test that this compiles...

   struct foo : TAO_PEGTL_STRING( "foo" )
   {
   };

   struct foobar : tao::TAO_PEGTL_NAMESPACE::sor< TAO_PEGTL_STRING( "foo" ), TAO_PEGTL_STRING( "bar" ) >
   {
   };

   static_assert( std::is_same< TAO_PEGTL_STRING( "Hello" ), tao::TAO_PEGTL_NAMESPACE::string< 'H', 'e', 'l', 'l', 'o' > >::value, "TAO_PEGTL_STRING broken" );
   static_assert( !std::is_same< TAO_PEGTL_ISTRING( "Hello" ), tao::TAO_PEGTL_NAMESPACE::string< 'H', 'e', 'l', 'l', 'o' > >::value, "TAO_PEGTL_ISTRING broken" );
   static_assert( std::is_same< TAO_PEGTL_ISTRING( "Hello" ), tao::TAO_PEGTL_NAMESPACE::istring< 'H', 'e', 'l', 'l', 'o' > >::value, "TAO_PEGTL_ISTRING broken" );

   static_assert( std::is_same< TAO_PEGTL_KEYWORD( "private" ), tao::TAO_PEGTL_NAMESPACE::keyword< 'p', 'r', 'i', 'v', 'a', 't', 'e' > >::value, "TAO_PEGTL_KEYWORD broken" );

   // Strings may even contain embedded nulls

   static_assert( std::is_same< TAO_PEGTL_STRING( "Hello, w\0rld!" ), tao::TAO_PEGTL_NAMESPACE::string< 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 0, 'r', 'l', 'd', '!' > >::value, "TAO_PEGTL_STRING broken" );

   // The strings currently have a maximum length of 512 characters.

   using namespace tao::TAO_PEGTL_NAMESPACE::alphabet;  // NOLINT
   static_assert( std::is_same< TAO_PEGTL_STRING( "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" ),
                                tao::TAO_PEGTL_NAMESPACE::string< a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z > >::value,
                  "TAO_PEGTL_STRING broken" );

}  // namespace test

int main()
{
   return 0;
}
