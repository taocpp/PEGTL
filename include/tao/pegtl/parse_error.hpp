// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_PARSE_ERROR_HPP
#define TAO_PEGTL_PARSE_ERROR_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

#include "config.hpp"
#include "parse_error_base.hpp"
#include "position.hpp"

#include "internal/extract_position.hpp"
#include "internal/stream_to_string.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct disambiguate_t
   {
      // TODO: Integrate extract_position into parse_error with SFINAE?
   };

   template< typename Position >
   class parse_error_template
      : public parse_error_base
   {
   public:
      using position_t = Position;

      template< typename Object >
      parse_error_template( const std::string& msg, const Object& obj )
         : parse_error_template( msg, internal::extract_position( obj ), disambiguate_t() )
      {}

      [[nodiscard]] const position_t& position_object() const noexcept
      {
         return m_position;
      }

   protected:
      const position_t m_position;

      parse_error_template( const std::string& msg, const Position& pos, const disambiguate_t /*unused*/ )
         : parse_error_base( msg, internal::stream_to_string( pos ) ),
           m_position( pos )
      {}
   };

   template< typename Object >
   parse_error_template( const std::string&, const Object& ) -> parse_error_template< std::decay_t< decltype( internal::extract_position( std::declval< Object >() ) ) > >;

   using parse_error = parse_error_template< position >;  // Temporary -- when the inputs are templated over the position class the parse_error_template will be renamed to parse_error.

}  // namespace TAO_PEGTL_NAMESPACE

#endif
