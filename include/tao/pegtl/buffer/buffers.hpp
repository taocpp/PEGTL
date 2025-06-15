// Copyright (c) 2023-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_BUFFERS_HPP
#define TAO_PEGTL_BUFFER_BUFFERS_HPP

#include <cstdlib>

#include "../config.hpp"

#if !defined TAO_PEGTL_DEFAULT_BUFFER_SIZE
#define TAO_PEGTL_DEFAULT_BUFFER_SIZE 4000
#endif

#if !defined TAO_PEGTL_DEFAULT_CHUNK_SIZE
#define TAO_PEGTL_DEFAULT_CHUNK_SIZE 1000
#endif

#include "cstream_reader.hpp"
#include "cstring_reader.hpp"
#include "endless_reader.hpp"
#include "istream_reader.hpp"

#include "dynamic_buffer.hpp"
#include "external_buffer.hpp"
#include "static_buffer.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   inline constexpr std::size_t default_buffer_size = TAO_PEGTL_DEFAULT_BUFFER_SIZE;
   inline constexpr std::size_t default_chunk_size = TAO_PEGTL_DEFAULT_CHUNK_SIZE;

   using dynamic_cstream_buffer = internal::dynamic_buffer< char, internal::cstream_reader >;
   using dynamic_cstring_buffer = internal::dynamic_buffer< char, internal::cstring_reader >;
   using dynamic_endless_buffer = internal::dynamic_buffer< char, internal::endless_reader >;
   using dynamic_istream_buffer = internal::dynamic_buffer< char, internal::istream_reader >;

   using external_cstream_buffer = internal::external_buffer< char, internal::cstream_reader >;
   using external_cstring_buffer = internal::external_buffer< char, internal::cstring_reader >;
   using external_endless_buffer = internal::external_buffer< char, internal::endless_reader >;
   using external_istream_buffer = internal::external_buffer< char, internal::istream_reader >;

   template< std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using static_cstream_buffer = internal::static_buffer< char, internal::cstream_reader, BufferSize, ChunkSize >;
   template< std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using static_cstring_buffer = internal::static_buffer< char, internal::cstring_reader, BufferSize, ChunkSize >;
   template< std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using static_endless_buffer = internal::static_buffer< char, internal::endless_reader, BufferSize, ChunkSize >;
   template< std::size_t BufferSize = default_buffer_size, std::size_t ChunkSize = default_chunk_size >
   using static_istream_buffer = internal::static_buffer< char, internal::istream_reader, BufferSize, ChunkSize >;

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
