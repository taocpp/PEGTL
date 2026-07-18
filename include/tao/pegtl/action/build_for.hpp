// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_BUILD_FOR_HPP
#define TAO_PEGTL_ACTION_BUILD_FOR_HPP

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule, typename Producer >
   struct build_for
   {
      using rules_t = type_list< Rule >;

      template< typename MatchedRule >
      using producer_t = Producer;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
