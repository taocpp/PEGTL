// Copyright (c) 2021-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SEP_PAD_HPP
#define TAO_PEGTL_INTERNAL_SEP_PAD_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "seq.hpp"
#include "star.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... >
   struct sep_pad;

   template< typename... Ts, typename Sep, typename Pad, typename Rule, typename... Rules >
   struct sep_pad< type_list< Ts... >, Sep, Pad, Rule, Rules... >
      : sep_pad< type_list< Ts..., Rule, internal::star< Pad >, Sep, internal::star< Pad > >, Sep, Pad, Rules... >
   {};

   template< typename... Ts, typename Sep, typename Pad, typename Rule >
   struct sep_pad< type_list< Ts... >, Sep, Pad, Rule >
      : seq< Ts..., Rule >
   {};

   template< typename Sep, typename Pad >
   struct sep_pad< type_list<>, Sep, Pad >
      : success
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
