// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_FORWARD_HPP
#define TAO_PEGTL_FORWARD_HPP

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T, typename = void >
   struct traits;

   template< typename... Rules >
   struct rule_list
   {
      static constexpr std::size_t size = sizeof...( Rules );
   };

   using empty_list = rule_list<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
