// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_OFFSET_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_OFFSET_HPP

#include <utility>

#include "../config.hpp"

#include "apply_offset.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< bool, typename Input >
   class input_with_offset
      : public Input
   {
   public:
      using data_t = typename Input::data_t;
      using error_position_t = typename Input::error_position_t;
      using offset_position_t = typename Input::offset_position_t;
      using rewind_position_t = typename Input::rewind_position_t;

      template< typename... Ts >
      explicit input_with_offset( offset_position_t&& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_offset( std::move( s ) )
      {}

      template< typename... Ts >
      explicit input_with_offset( const offset_position_t& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_offset( s )
      {}

      [[nodiscard]] auto current_position() const
      {
         auto pos = Input::current_position();
         apply_offset( pos, m_offset );
         return pos;
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const
      {
         auto pos = Input::previous_position( saved );
         apply_offset( pos, m_offset );
         return pos;
      }

      [[nodiscard]] const offset_position_t& direct_offset() const noexcept
      {
         return m_offset;
      }

   protected:
      offset_position_t m_offset;
   };

   template< typename Input >
   class input_with_offset< false, Input >
      : public Input
   {
      using Input::Input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
