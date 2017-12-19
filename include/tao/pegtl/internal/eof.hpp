// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_EOF_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_EOF_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../analysis/generic.hpp"

namespace tao
{
   namespace pegtl
   {
      namespace internal
      {
         struct eof
         {
            using analyze_t = analysis::generic< analysis::rule_type::OPT >;

            template< typename Input >
            static bool match( Input& in )
            {
               return in.empty();
            }
         };

         template<>
         struct skip_control< eof > : std::true_type
         {
         };

      }  // namespace internal

   }  // namespace pegtl

}  // namespace tao

#endif
