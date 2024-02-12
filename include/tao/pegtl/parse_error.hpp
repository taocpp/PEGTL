// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_PARSE_ERROR_HPP
#define TAO_PEGTL_PARSE_ERROR_HPP

#include <string>
#include <type_traits>
#include <utility>

#include "config.hpp"
#include "parse_error_base.hpp"

#include "internal/extract_position.hpp"
#include "internal/stream_to_string.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename Position >
   class parse_error
      : public parse_error_base
   {
   public:
      using position_t = Position;

      parse_error( const std::string& msg, Position&& pos )
         : parse_error_base( msg, internal::stream_to_string( pos ) ),
           m_position( std::move( pos ) )
      {}

      parse_error( const std::string& msg, const Position& pos )
         : parse_error_base( msg, internal::stream_to_string( pos ) ),
           m_position( pos )
      {}

      [[nodiscard]] const position_t& position_object() const noexcept
      {
         return m_position;
      }

   protected:
      const position_t m_position;
   };

   template< typename Object >
   [[noreturn]] void throw_parse_error( const std::string& msg, const Object& obj )
   {
      auto pos = internal::extract_position( obj );
      throw parse_error< std::decay_t< decltype( pos ) > >( msg, std::move( pos ) );
   }

   template< typename Object >
   [[noreturn]] void throw_with_nested_parse_error( const std::string& msg, const Object& obj )
   {
      auto pos = internal::extract_position( obj );
      std::throw_with_nested( parse_error< std::decay_t< decltype( pos ) > >( msg, std::move( pos ) ) );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
