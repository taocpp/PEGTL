// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_IPV4_HPP
#define TAO_PEGTL_EXAMPLE_IPV4_HPP

#include <cstdint>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "../extra/charconv.hpp"

namespace TAO_PEGTL_NAMESPACE::ipv4
{
   // clang-format off
   struct dot : one< '.' > {};
   struct octet : from_chars_nothrow< std::uint8_t > {};
   struct address : seq< octet, dot, octet, dot, octet, dot, octet > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::ipv4

#endif
