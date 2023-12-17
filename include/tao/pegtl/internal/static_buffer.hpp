// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_STATIC_BUFFER_HPP
#define TAO_PEGTL_INTERNAL_STATIC_BUFFER_HPP

#include <array>
#include <cstddef>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Data, typename Reader, std::size_t Size, std::size_t Chunk >
   class static_buffer
   {
   public:
      using data_t = Data;

      static_assert( Chunk > 0 );
      static_assert( Chunk < Size );

      template< typename... As >
      static_buffer( As&&... as )
         : m_reader( std::forward< As >( as )... )
      {}

      static_buffer( static_buffer&& ) = delete;
      static_buffer( const static_buffer& ) = delete;

      ~static_buffer() = default;

      void operator=( static_buffer&& ) = delete;
      void operator=( const static_buffer& ) = delete;

      [[nodiscard]] Data* mutable_begin() noexcept
      {
         return m_buffer.data();
      }

      [[nodiscard]] const Data* buffer_begin() const noexcept
      {
         return m_buffer.data();
      }

      [[nodiscard]] std::size_t buffer_capacity() const noexcept
      {
         return m_buffer.size();
      }

      [[nodiscard]] std::size_t buffer_chunk_size() const noexcept
      {
         return Chunk;
      }

   protected:
      std::array< Data, Size > m_buffer;

      Reader m_reader;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
