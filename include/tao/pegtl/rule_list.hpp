// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_RULE_LIST_HPP
#define TAO_PEGTL_RULE_LIST_HPP

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   struct rule_list
   {};

   using empty_list = rule_list<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
