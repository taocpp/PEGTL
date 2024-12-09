// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_OPT_HPP
#define TAO_PEGTL_INTERNAL_OPT_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "partial.hpp"
#include "seq.hpp"
#include "success.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Rules >
   struct opt
      : opt< seq< Rules... > >
   {};

   template< typename Rule >
   struct opt< Rule >
      : partial< Rule >
   {
      using rule_t = opt;
   };

   template<>
   struct opt<>
      : success
   {};

   template< typename... Rules >
   inline constexpr bool enable_control< opt< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

// FCOV_EXCL_FILE
#endif
