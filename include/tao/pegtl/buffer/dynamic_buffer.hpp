// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_DYNAMIC_BUFFER_HPP
#define TAO_PEGTL_BUFFER_DYNAMIC_BUFFER_HPP

#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Reader >
   class dynamic_buffer
   {
   public:
      using data_t = Data;

      template< typename... As >
      dynamic_buffer( const std::size_t size, const std::size_t chunk, As&&... as )
         : m_size( size ),
           m_chunk( chunk ),
           m_buffer( new Data[ size ] ),
           m_reader( std::forward< As >( as )... )
      {
         assert( chunk > 0 );
         assert( size > chunk );
      }

      dynamic_buffer( dynamic_buffer&& ) = delete;
      dynamic_buffer( const dynamic_buffer& ) = delete;

      ~dynamic_buffer() = default;

      void operator=( dynamic_buffer&& ) = delete;
      void operator=( const dynamic_buffer& ) = delete;

      [[nodiscard]] Data* mutable_start() noexcept
      {
         return m_buffer.get();
      }

      [[nodiscard]] const Data* buffer_start() const noexcept
      {
         return m_buffer.get();
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
      const std::size_t m_size;
      const std::size_t m_chunk;
      const std::unique_ptr< Data[] > m_buffer;

      Reader m_reader;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
