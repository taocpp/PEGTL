// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_INPUTS_HPP
#define TAO_PEGTL_BUFFER_INPUTS_HPP

#include "../ascii.hpp"
#include "../config.hpp"
#include "../system.hpp"

#include "buffers.hpp"

#include "buffer_input.hpp"
#include "buffer_input_with_source.hpp"

#include "text_buffer_input.hpp"
#include "text_buffer_input_with_source.hpp"

#include "../internal/input_with_funcs.hpp"
#include "../internal/input_with_lines.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   using tao_buffer_eol = TAO_PEGTL_DEFAULT_EOL;  // Details might change.

   template< typename Buffer, typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using buffer_input = internal::input_with_funcs< internal::input_with_lines< Eol, internal::buffer_input_with_source< Buffer, InputSource, ErrorSource > > >;

   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_cstream_input = buffer_input< dynamic_cstream_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_cstring_input = buffer_input< dynamic_cstring_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_endless_input = buffer_input< dynamic_endless_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_istream_input = buffer_input< dynamic_istream_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_cstream_input = buffer_input< static_cstream_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_cstring_input = buffer_input< static_cstring_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_endless_input = buffer_input< static_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_istream_input = buffer_input< static_istream_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

   template< typename Buffer, typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using text_buffer_input = internal::input_with_funcs< internal::text_buffer_input_with_source< Buffer, Eol, InputSource, ErrorSource > >;

   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_text_cstream_input = text_buffer_input< dynamic_cstream_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_text_cstring_input = text_buffer_input< dynamic_cstring_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_text_endless_input = text_buffer_input< dynamic_endless_buffer, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource >
   using dynamic_text_istream_input = text_buffer_input< dynamic_istream_buffer, Eol, InputSource, ErrorSource >;

   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_text_cstream_input = text_buffer_input< static_cstream_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_text_cstring_input = text_buffer_input< static_cstring_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_text_endless_input = text_buffer_input< static_endless_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;
   template< typename Eol = tao_buffer_eol, typename InputSource = void, typename ErrorSource = InputSource, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_text_istream_input = text_buffer_input< static_istream_buffer< BufferSize, ChunkSize >, Eol, InputSource, ErrorSource >;

}  // namespace TAO_PEGTL_NAMESPACE

// FCOV_EXCL_FILE
#endif
