// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_SOR_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_SOR_HH

#include "../config.hh"

#include "skip_control.hh"

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

#include "../analysis/generic.hh"

#include "integer_sequence.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename ... Rules >
      struct sor
           : sor< index_sequence_for< Rules ... >, Rules ... > {};

      template< std::size_t ... Indices, typename ... Rules >
      struct sor< index_sequence< Indices ... >, Rules ... >
      {
         using analyze_t = analysis::generic< analysis::rule_type::SOR, Rules ... >;

         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
#ifdef __cpp_fold_expressions
            return ( Control< Rules >::template match< A, ( Indices == ( sizeof...( Rules ) - 1 ) ) ? M : rewind_mode::REQUIRED, Action, Control >( in, st ... ) || ... );
#else
            bool result = false;
            using swallow = bool[];
            (void)swallow{ result = result || Control< Rules >::template match< A, ( Indices == ( sizeof...( Rules ) - 1 ) ) ? M : rewind_mode::REQUIRED, Action, Control >( in, st ... ) ... };
            return result;
#endif
         }
      };

      template<>
      struct sor<>
            : trivial< false > {};

      template< typename ... Rules >
      struct skip_control< sor< Rules ... > > : std::true_type {};

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
