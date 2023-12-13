// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_SOURCE_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_SOURCE_HPP

#include <utility>

#include "../config.hpp"
#include "../position_with_source.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Source, typename Input >
   class input_with_source
      : public Input
   {
   public:
      using base_t = Input;
      using data_t = typename Input::data_t;
      using error_position_t = position_with_source< Source, typename Input::error_position_t >;
      using rewind_position_t = typename Input::rewind_position_t;

      template< typename S, typename... Ts >
      explicit input_with_source( S&& s, Ts&&... ts )
         : Input( std::forward< Ts >( ts )... ),
           m_source( std::forward< S >( s ) )
      {}

      [[nodiscard]] auto current_position() const
      {
         return error_position_t( m_source, Input::current_position() );
      }

      [[nodiscard]] auto previous_position( const rewind_position_t& saved ) const
      {
         return error_position_t( m_source, Input::previous_position( saved ) );
      }

      [[nodiscard]] const Source& direct_source() const noexcept
      {
         return m_source;
      }

   protected:
      Source m_source;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
