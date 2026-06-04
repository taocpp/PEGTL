// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <ostream>
#include <string>

#include <tao/pegtl/extra/unescape.hpp>

struct input
{
   [[nodiscard]] const char* begin() const noexcept
   {
      return "0";
   }
};

std::ostream& operator<<( std::ostream& os, const input& /*unused*/ )
{
   return os;
}

int main()
{
   std::string out;
   const input in;
#if TAO_PEGTL_COMPILE_ACCEPT
   TAO_PEGTL_NAMESPACE::unescape_unicode< 1 >::apply( in, out );
#else
   // include/tao/pegtl/extra/internal/unhex_utility.hpp
   // static_assert( N >= 1 );
   TAO_PEGTL_NAMESPACE::unescape_unicode< 0 >::apply( in, out );
#endif
   return out.empty() ? 1 : 0;
}
