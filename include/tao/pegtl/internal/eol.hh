// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_EOL_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_EOL_HH

#include "../config.hh"

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         struct eol
         {
            using analyze_t = analysis::generic< analysis::rule_type::ANY >;

            template< typename Input >
            static bool match( Input & in )
            {
               using eol_t = typename Input::eol_t;
               return eol_t::match( in ).first;
            }
         };

         template<>
         struct skip_control< eol > : std::true_type {};

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
