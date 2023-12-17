// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_COPY_INPUT_HPP
#define TAO_PEGTL_INTERNAL_COPY_INPUT_HPP

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string>
#include <utility>

#include "../config.hpp"
#include "../count_position.hpp"
#include "../pointer_position.hpp"

#include "rewind_guard.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Container = std::string >
   class copy_input
   {
   public:
      using data_t = typename Container::value_type;
      using error_position_t = count_position;
      using rewind_position_t = pointer_position< data_t >;

      copy_input( const data_t* in_begin, const data_t* in_end )
         : m_container( in_begin, in_end ),
           m_current( m_container.data() )
      {}

      copy_input( const data_t* in_begin, const std::size_t in_size )
         : copy_input( in_begin, in_begin + in_size )
      {}

      explicit copy_input( Container&& in_data ) noexcept
         : m_container( std::move( in_data ) ),
           m_current( m_container.data() )
      {}

      explicit copy_input( const Container& in_data )
         : m_container( in_data ),
           m_current( m_container.data() )
      {}

      template< std::size_t Size >
      explicit copy_input( const std::array< data_t, Size >& in_array )
         : copy_input( in_array.data(), in_array.size() )
      {}

      explicit copy_input( const std::initializer_list< data_t >& init )
         : m_container( init ),
           m_current( m_container.data() )
      {}

      [[nodiscard]] bool empty() const noexcept
      {
         return size() == 0;
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return end() - current();
      }

      [[nodiscard]] const data_t* start() const noexcept
      {
         return m_container.data();
      }

      [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
      {
         return m_current + offset;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return saved.data;
      }

      [[nodiscard]] const data_t* end() const noexcept
      {
         return m_container.data() + m_container.size();
      }

      void restart() noexcept
      {
         m_current = m_container.data();
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         m_current += count;
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, copy_input >( this );
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( m_current );
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         m_current = previous( saved );
      }

      [[nodiscard]] auto current_position() const noexcept
      {
         return previous_position( rewind_position() );
      }

      // [[nodiscard]] auto previous_position( const error_position_t saved ) const noexcept
      // {
      //    return saved;
      // }

      [[nodiscard]] auto previous_position( const rewind_position_t saved ) const noexcept
      {
         return error_position_t( saved.data - m_container.data() );
      }

      void private_set_current( const data_t* in_current ) noexcept
      {
         m_current = in_current;
      }

   protected:
      Container m_container;
      const data_t* m_current;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
