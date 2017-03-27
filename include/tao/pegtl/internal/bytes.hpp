// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_BYTES_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_BYTES_HPP

#include "../config.hpp"

#include "skip_control.hpp"

#include "../analysis/counted.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< unsigned Num >
         struct bytes
         {
            using analyze_t = analysis::counted< analysis::rule_type::ANY, Num >;

            // suppress warning with GCC 4.7
            template< typename T1, typename T2 >
            static inline bool dummy_greater_or_equal( const T1 a, const T2 b )
            {
               return a >= b;
            }

            template< typename Input >
            static bool match( Input & in )
            {
               if ( dummy_greater_or_equal( in.size( Num ), Num ) ) {
                  in.bump( Num );
                  return true;
               }
               return false;
            }
         };

         template< unsigned Num >
         struct skip_control< bytes< Num > > : std::true_type {};

      } // namespace internal

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#endif
