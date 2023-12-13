// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_INPUT_HPP
#define TAO_PEGTL_INTERNAL_LAZY_INPUT_HPP

#include <cstddef>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"
#include "../text_position.hpp"

#include "input_with_lines.hpp"
#include "rewind_adapt.hpp"
#include "text_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class lazy_input
      : public input_with_lines< Eol, Input >
   {
   public:
      using base_t = input_with_lines< Eol, Input >;
      using data_t = typename Input::data_t;
      using error_position_t = text_position;
      using rewind_position_t = typename Input::rewind_position_t;

      using eol_rule = Eol;

      using input_with_lines< Eol, Input >::input_with_lines;

      [[nodiscard]] auto current_position() const
      {
         return previous_position( this->rewind_position() );
      }

      [[nodiscard]] auto previous_position( const rewind_position_t saved ) const
      {
         error_position_t pos;
         text_eol_scan< Eol >( pos, this->start(), rewind_adapt( this->start(), saved ) );
         pos.count = rewind_adapt( this->start(), saved ) - this->start();
         return pos;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
