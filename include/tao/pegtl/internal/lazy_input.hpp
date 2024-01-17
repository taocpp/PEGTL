// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
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
#include "lazy_eol_scan.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class lazy_input
      : public Input
   {
   public:
      using data_t = typename Input::data_t;
      using error_position_t = text_position;
      using offset_position_t = text_position;
      using rewind_position_t = typename Input::rewind_position_t;

      using eol_rule = Eol;

      using Input::Input;

      [[nodiscard]] const data_t* previous( const error_position_t saved ) const noexcept
      {
         return this->start() + saved.count;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return saved.data;
      }

      [[nodiscard]] auto current_position() const
      {
         return previous_position( this->rewind_position() );
      }

      [[nodiscard]] auto previous_position( const rewind_position_t saved ) const
      {
         error_position_t pos;
         const std::size_t count = this->previous( saved ) - this->start();
         lazy_eol_scan< Eol >( pos, this->start(), count );
         pos.count = count;
         return pos;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
