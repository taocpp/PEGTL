// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_LIST_MUST_HPP
#define TAO_PEGTL_INTERNAL_LIST_MUST_HPP

#include "../config.hpp"

#include "must.hpp"
#include "seq.hpp"
#include "star.hpp"

#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Sep >
   struct list_must
      : seq< Rule, star< Sep, must< Rule > > >
   {
      using rule_t = list_must;
      using subs_t = rule_list< Rule, Sep >;
      using impl_t = seq< Rule, star< Sep, must< Rule > > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
