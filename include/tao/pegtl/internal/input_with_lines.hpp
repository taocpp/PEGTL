// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_LINES_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_LINES_HPP

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class input_with_lines
      : public Input
   {
   public:
      using eol_rule = Eol;

      using Input::Input;
   };

   template< typename Input >
   class input_with_lines< void, Input >
      : public Input
   {
   public:
      using Input::Input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
