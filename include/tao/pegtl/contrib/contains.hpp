// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_CONTAINS_HPP
#define TAO_PEGTL_CONTRIB_CONTAINS_HPP

#include <type_traits>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Rule >
   struct contains
   {
      template< typename Visited >
      struct visitor
      {
         static void visit( bool& result )
         {
            if constexpr( std::is_same_v< Rule, Visited > ) {
               result = true;
            }
         }
      };
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
