// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_IPV6_HPP
#define TAO_PEGTL_EXAMPLE_IPV6_HPP

#include <cstdint>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "ipv4.hpp"

namespace TAO_PEGTL_NAMESPACE::ipv6
{
   // clang-format off
   struct colon : one< ':' > {};
   struct dcolon : two< ':' > {};

   struct h16 : rep_min_max< 1, 4, xdigit > {};
   struct ls32 : sor< seq< h16, colon, h16 >, ipv4::address > {};

   namespace internal
   {
      template< std::size_t Max, typename Rule, typename Sep >
      struct list_max : opt< Rule, rep_opt< Max - 1, Sep, Rule > > {};

      template< typename Rule, typename Sep >
      struct list_max< 0, Rule, Sep > : success {};

      template< std::size_t N >
      struct impl : seq< list_max< 5 - N, h16, colon >, dcolon, rep< N, h16, colon >, ls32 > {};

   }  // namespace internal

   struct address : sor< seq< rep< 6, h16, colon >, ls32 >,
                         internal::impl< 5 >,
                         internal::impl< 4 >,
                         internal::impl< 3 >,
                         internal::impl< 2 >,
                         internal::impl< 1 >,
                         internal::impl< 0 >,
                         seq< internal::list_max< 6, h16, colon >, dcolon, h16 >,
                         seq< internal::list_max< 7, h16, colon >, dcolon > > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::ipv6

#endif
