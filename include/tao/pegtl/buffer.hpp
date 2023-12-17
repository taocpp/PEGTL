// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_BUFFER_HPP
#define TAO_PEGTL_BUFFER_HPP

#include <cstddef>
#include <istream>

#include "config.hpp"

#include "internal/buffer_input.hpp"
#include "internal/dynamic_buffer.hpp"
#include "internal/static_buffer.hpp"

#include "internal/input_with_peeks.hpp"
#include "internal/input_with_source.hpp"

#include "internal/cstream_reader.hpp"
#include "internal/cstring_reader.hpp"
#include "internal/istream_reader.hpp"

#if !defined TAO_PEGTL_DEFAULT_BUFFER_SIZE
#define TAO_PEGTL_DEFAULT_BUFFER_SIZE 4000
#endif

#if !defined TAO_PEGTL_DEFAULT_CHUNK_SIZE
#define TAO_PEGTL_DEFAULT_CHUNK_SIZE 1000
#endif

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Reader >
   struct dynamic_input
      : internal::input_with_peeks< internal::buffer_input< internal::dynamic_buffer< char, Reader > > >
   {
      using internal::input_with_peeks< internal::buffer_input< internal::dynamic_buffer< char, Reader > > >::input_with_peeks;
   };

   dynamic_input( const std::size_t, const std::size_t, std::FILE* ) -> dynamic_input< internal::cstream_reader >;
   dynamic_input( const std::size_t, const std::size_t, const char* ) -> dynamic_input< internal::cstring_reader >;
   dynamic_input( const std::size_t, const std::size_t, std::istream& ) -> dynamic_input< internal::istream_reader >;

   using dynamic_cstream_input = dynamic_input< internal::cstream_reader >;
   using dynamic_cstring_input = dynamic_input< internal::cstring_reader >;
   using dynamic_istream_input = dynamic_input< internal::istream_reader >;

   template< typename Reader, std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   struct static_input
      : internal::input_with_peeks< internal::buffer_input< internal::static_buffer< char, Reader, BufferSize, ChunkSize > > >
   {
      using internal::input_with_peeks< internal::buffer_input< internal::static_buffer< char, Reader, BufferSize, ChunkSize > > >::input_with_peeks;
   };

   static_input( std::FILE* ) -> static_input< internal::cstream_reader >;
   static_input( const char* ) -> static_input< internal::cstring_reader >;
   static_input( std::istream& ) -> static_input< internal::istream_reader >;

   template< std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_cstream_input = static_input< internal::cstream_reader, BufferSize, ChunkSize >;

   template< std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_cstring_input = static_input< internal::cstring_reader, BufferSize, ChunkSize >;

   template< std::size_t BufferSize = TAO_PEGTL_DEFAULT_BUFFER_SIZE, std::size_t ChunkSize = TAO_PEGTL_DEFAULT_CHUNK_SIZE >
   using static_istream_input = static_input< internal::istream_reader, BufferSize, ChunkSize >;

}  // namespace TAO_PEGTL_NAMESPACE

#include "analyze_traits.hpp"

#include "internal/discard.hpp"
#include "internal/is_buffer.hpp"
#include "internal/require.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // clang-format off
   struct discard : internal::discard {};
   struct is_buffer : internal::is_buffer {};
   template< unsigned Amount > struct require : internal::require< Amount > {};
   // clang-format on

   template< typename Name >
   struct analyze_traits< Name, internal::discard >
      : analyze_opt_traits<>
   {};

   template< typename Name >
   struct analyze_traits< Name, internal::is_buffer >
      : analyze_opt_traits<>
   {};

   template< typename Name, unsigned Amount >
   struct analyze_traits< Name, internal::require< Amount > >
      : analyze_opt_traits<>
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#include "discard_input.hpp"
#include "discard_input_on_failure.hpp"
#include "discard_input_on_success.hpp"

#endif
