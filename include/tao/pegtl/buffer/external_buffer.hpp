// Copyright (c) 2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_EXTERNAL_BUFFER_HPP
#define TAO_PEGTL_BUFFER_EXTERNAL_BUFFER_HPP

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Reader >
   class external_buffer
   {
   public:
      using data_t = Data;

      template< typename... As >
      external_buffer( Data* data, const std::size_t size, const std::size_t chunk, As&&... as )
         : m_data( data ),
           m_size( size ),
           m_chunk( chunk ),
           m_reader( std::forward< As >( as )... )
      {
         assert( chunk > 0 );
         assert( chunk < size );
      }

      external_buffer( external_buffer&& ) = delete;
      external_buffer( const external_buffer& ) = delete;

      ~external_buffer() = default;

      void operator=( external_buffer&& ) = delete;
      void operator=( const external_buffer& ) = delete;

      [[nodiscard]] Data* mutable_start() noexcept
      {
         return m_data;
      }

      [[nodiscard]] const Data* buffer_start() const noexcept
      {
         return m_data;
      }

      [[nodiscard]] std::size_t buffer_capacity() const noexcept
      {
         return m_size;
      }

      [[nodiscard]] std::size_t buffer_chunk_size() const noexcept
      {
         return m_chunk;
      }

      [[nodiscard]] const Reader& direct_reader() const noexcept
      {
         return m_reader;
      }

   protected:
      Data* m_data;

      const std::size_t m_size;
      const std::size_t m_chunk;

      Reader m_reader;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
