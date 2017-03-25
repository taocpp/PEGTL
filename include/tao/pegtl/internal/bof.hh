// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_BOF_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_BOF_HH

#include "../config.hh"

#include "skip_control.hh"

#include "../analysis/generic.hh"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         struct bof
         {
            using analyze_t = analysis::generic< analysis::rule_type::OPT >;

            template< typename Input >
            static bool match( Input & in )
            {
               return in.byte() == 0;
            }
         };

         template<>
         struct skip_control< bof > : std::true_type {};

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
