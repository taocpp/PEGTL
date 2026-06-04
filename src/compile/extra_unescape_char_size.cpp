// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <string>

#include <tao/pegtl.hpp>
#include <tao/pegtl/extra/unescape.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct input
{
   [[nodiscard]] char peek_char() const noexcept
   {
      return 'a';
   }
};

int main()
{
   std::string out;
   const input in;
#if TAO_PEGTL_COMPILE_ACCEPT
   pegtl::unescape_char< 'x', 'y' >::template apply< pegtl::one< 'a', 'b' > >( in, out );
#else
   // include/tao/pegtl/extra/unescape.hpp
   // static_assert( sizeof...( Qs ) == sizeof...( Rs ), "Size mismatch between escaped characters and their mapping!" );
   pegtl::unescape_char< 'x' >::template apply< pegtl::one< 'a', 'b' > >( in, out );
#endif
   return out.empty() ? 1 : 0;
}
