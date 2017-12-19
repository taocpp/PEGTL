// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_EOLF_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_EOLF_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         struct eolf
         {
            using analyze_t = analysis::generic< analysis::rule_type::OPT >;

            template< typename Input >
            static bool match( Input& in )
            {
               using eol_t = typename Input::eol_t;
               const auto p = eol_t::match( in );
               return p.first || ( !p.second );
            }
         };

         template<>
         struct skip_control< eolf > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
