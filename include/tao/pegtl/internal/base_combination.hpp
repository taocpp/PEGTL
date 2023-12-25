// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_BASE_COMBINATION_HPP
#define TAO_PEGTL_INTERNAL_BASE_COMBINATION_HPP

#include "../config.hpp"

#include "input_with_fakes.hpp"
#include "input_with_lines.hpp"
#include "input_with_peeks.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class base_combination
      : public input_with_fakes< input_with_peeks< input_with_lines< Eol, Input > > >
   {
   public:
      using input_with_fakes< input_with_peeks< input_with_lines< Eol, Input > > >::input_with_fakes;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
