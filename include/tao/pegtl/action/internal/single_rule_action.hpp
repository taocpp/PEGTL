// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_SINGLE_RULE_ACTION_HPP
#define TAO_PEGTL_ACTION_INTERNAL_SINGLE_RULE_ACTION_HPP

#include <type_traits>

#include "../../config.hpp"
#include "../../nothing.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename SelectedRule, typename SelectedAction >
   struct single_rule_action
   {
      template< typename Rule >
      struct type
         : std::conditional_t< std::is_same_v< Rule, SelectedRule >, SelectedAction, nothing< Rule > >
      {};
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
