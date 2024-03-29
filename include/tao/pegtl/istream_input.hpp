// Copyright (c) 2017-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ISTREAM_INPUT_HPP
#define TAO_PEGTL_ISTREAM_INPUT_HPP

#include <istream>

#include "buffer_input.hpp"
#include "config.hpp"
#include "eol.hpp"

#include "internal/istream_reader.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Eol = eol::lf_crlf, std::size_t Chunk = 64 >
   struct istream_input
      : buffer_input< internal::istream_reader, Eol, std::string, Chunk >
   {
      template< typename T >
      istream_input( std::istream& in_stream, const std::size_t in_maximum, T&& in_source )
         : buffer_input< internal::istream_reader, Eol, std::string, Chunk >( std::forward< T >( in_source ), in_maximum, in_stream )
      {}
   };

   template< typename... Ts >
   istream_input( Ts&&... ) -> istream_input<>;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
