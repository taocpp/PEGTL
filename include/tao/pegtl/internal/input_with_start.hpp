// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUT_WITH_START_HPP
#define TAO_PEGTL_INTERNAL_INPUT_WITH_START_HPP

#include <cstddef>
#include <utility>

#include "../config.hpp"
#include "../count_position.hpp"

#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data = char >
   class input_with_start
      : public view_input< Data >
   {
   public:
      using base_t = view_input< Data >;
      using data_t = Data;
      using error_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;

      template< typename... As >
      explicit input_with_start( As&&... as ) noexcept
         : view_input< Data >( std::forward< As >( as )... ),
           m_start( this->current() )
      {}

      [[nodiscard]] const data_t* start() const noexcept
      {
         return m_start;
      }

      void restart() noexcept
      {
         this->m_current = m_start;
      }

      [[nodiscard]] auto current_position() const noexcept
      {
         return previous_position( this->rewind_position() );
      }

      // [[nodiscard]] auto previous_position( const error_position_t saved ) const noexcept
      // {
      //    return saved;
      // }

      [[nodiscard]] auto previous_position( const rewind_position_t saved ) const noexcept
      {
         return count_position( saved.data - m_start );
      }

   protected:
      const data_t* m_start;
   };

}  // TAO_PEGTL_NAMESPACE::internal

#endif
