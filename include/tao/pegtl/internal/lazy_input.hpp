// Copyright (c) 2021-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_LAZY_INPUT_HPP
#define TAO_PEGTL_INTERNAL_LAZY_INPUT_HPP

#include <algorithm>
#include <cstddef>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#if defined( __cpp_exceptions )
#include "../parse_error.hpp"
#endif
#include "../rewind_mode.hpp"
#include "../text_position.hpp"

#include "any.hpp"
#include "at.hpp"
#include "choose_lazy_traits.hpp"
#include "eof.hpp"
#include "input_with_lines.hpp"
#include "scan_input.hpp"
#include "sor.hpp"
#include "until.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   struct lazy_input
      : Input
   {
      using data_t = typename Input::data_t;
      using error_position_t = text_position;
      using offset_position_t = text_position;
      using rewind_position_t = typename Input::rewind_position_t;
#if defined( __cpp_exceptions )
      using parse_error_t = parse_error< error_position_t >;
#endif

      using eol_rule = Eol;

      using Input::Input;

      static_assert( !std::is_same_v< Eol, void > );

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
         scan_input< data_t > in( this->start(), count );
         choose_lazy_traits< Eol >( pos, in );
         return pos;
      }

      [[nodiscard]] const data_t* begin_of_line( const error_position_t& pos, const std::size_t max = 135 ) const noexcept
      {
         // assert( pos.column > 0 );
         return previous( pos ) - ( std::min )( max, pos.column - 1 );
      }

      [[nodiscard]] const data_t* end_of_line_or_file( const error_position_t& pos, const std::size_t max = 135 ) const noexcept
      {
         using grammar = until< at< sor< eof, eol_rule > >, any< typename eol_rule::eol_lazy_peek > >;
         const data_t* p = previous( pos );  // TODO: Start earlier?
         const std::size_t s = ( std::min )( max, std::size_t( this->end() - p ) );
         scan_input< data_t > in( p, s );
         if( normal< grammar >::template match< apply_mode::nothing, rewind_mode::optional, nothing, normal >( in ) ) {
            return in.current();
         }
         return in.current() + max;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
