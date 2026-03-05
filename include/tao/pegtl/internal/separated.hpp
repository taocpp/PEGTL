// Copyright (c) 2021-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_SEPARATED_HPP
#define TAO_PEGTL_INTERNAL_SEPARATED_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "seq.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... >
   struct separated;

   template< typename... Ts, typename Sep, typename Rule, typename... Rules >
   struct separated< type_list< Ts... >, Sep, Rule, Rules... >
      : separated< type_list< Ts..., Rule, Sep >, Sep, Rules... >
   {};

   template< typename... Ts, typename Sep, typename Rule >
   struct separated< type_list< Ts... >, Sep, Rule >
      : seq< Ts..., Rule >
   {};

   template< typename Sep >
   struct separated< type_list<>, Sep >
      : success
   {};

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
