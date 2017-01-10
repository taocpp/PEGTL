// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_RULE_CONJUNCTION_HH
#define PEGTL_INTERNAL_RULE_CONJUNCTION_HH

#include "../apply_mode.hh"
#include "../rewind_mode.hh"

namespace pegtl
{
   namespace internal
   {
      template< typename ... Rules >
      struct rule_conjunction
      {
         template< apply_mode A, rewind_mode M, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
#ifdef __cpp_fold_expressions
            return ( Control< Rules >::template match< A, M, Action, Control >( in, st ... ) && ... );
#else
            bool result = true;
            using swallow = bool[];
            (void)swallow{ result = result && Control< Rules >::template match< A, M, Action, Control >( in, st ... ) ..., true };
            return result;
#endif
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
