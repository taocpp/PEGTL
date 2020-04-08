// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_LIST_TAIL_HPP
#define TAO_PEGTL_INTERNAL_LIST_TAIL_HPP

#include "../config.hpp"

#include "list.hpp"
#include "opt.hpp"
#include "seq.hpp"

#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Sep >
   struct list_tail
      : seq< list< Rule, Sep >, opt< Sep > >
   {
      using rule_t = list_tail;
      using subs_t = rule_list< Rule, Sep >;
      using impl_t = seq< list< Rule, Sep >, opt< Sep > >;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
