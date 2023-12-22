// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_OFFSET_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_OFFSET_HPP

#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename OffsetPosition, typename Input >
   class input_with_offset
      : public Input
   {
   public:
      using data_t = typename Input::data_t;
      using error_position_t = typename Input::error_position_t;
      using rewind_position_t = typename Input::rewind_position_t;

      template< typename... Ts >
      explicit input_with_offset( OffsetPosition&& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_offset( std::move( s ) )
      {}

      template< typename... Ts >
      explicit input_with_offset( const OffsetPosition& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_offset( s )
      {}

      [[nodiscard]] auto current_position() const
      {
         auto pos = Input::current_position();
         pos += m_offset;
         return pos;
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const
      {
         auto pos = Input::previous_position( saved );
         pos += m_offset;
         return pos;
      }

      [[nodiscard]] const OffsetPosition& direct_offset() const noexcept
      {
         return m_offset;
      }

   protected:
      OffsetPosition m_offset;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
