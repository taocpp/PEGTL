// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_LIST_HPP
#define TAO_PEGTL_INTERNAL_LIST_HPP

#include "../config.hpp"

#include "seq.hpp"
#include "star.hpp"

#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Sep >
   struct list
      : seq< Rule, star< Sep, Rule > >
   {
      using rule_t = list;
      using subs_t = rule_list< Rule, Sep >;
      using impl_t = seq< Rule, star< Sep, Rule > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
