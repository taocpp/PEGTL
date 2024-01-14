// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_FUNCS_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_FUNCS_HPP

#include "../config.hpp"

#include "input_with_fakes.hpp"
#include "input_with_peeks.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   class input_with_funcs
      : public input_with_fakes< input_with_peeks< Input > >
   {
   public:
      using input_with_fakes< input_with_peeks< Input > >::input_with_fakes;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
