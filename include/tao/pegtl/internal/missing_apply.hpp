// Copyright (c) 2019-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MISSING_APPLY_HPP
#define TAO_PEGTL_INTERNAL_MISSING_APPLY_HPP

#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Control,
             template< typename... >
             class Action,
             typename ParseInput,
             typename... States >
   void missing_apply( ParseInput& in, States&&... st )
   {
      // This function only exists for better error messages, which means that it is only called when we know that it won't compile.
      // LCOV_EXCL_START
      auto m = in.template auto_rewind< rewind_mode::required >();
      (void)Control::template apply< Action >( m.inputerator(), in, st... );
      // LCOV_EXCL_STOP
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
