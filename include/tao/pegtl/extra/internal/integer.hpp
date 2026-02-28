// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_INTERNAL_INTEGER_HPP
#define TAO_PEGTL_EXTRA_INTERNAL_INTEGER_HPP

#include <limits>
#include <string_view>
#include <type_traits>

#include "../../config.hpp"
#include "../../eol_exclude_tag.hpp"

#include "ascii_utility.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
   [[nodiscard]] constexpr bool accumulate_digit( Integer& result, const char digit ) noexcept
   {
      // Assumes that digit is a digit as per is_ascii_digit(); returns false on overflow.

      static_assert( std::is_integral_v< Integer > );

      constexpr Integer cutoff = Maximum / 10;
      constexpr Integer cutlim = Maximum % 10;

      const Integer c = digit - '0';

      if( ( result > cutoff ) || ( ( result == cutoff ) && ( c > cutlim ) ) ) {
         return false;
      }
      result *= 10;
      result += c;
      return true;
   }

   template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
   [[nodiscard]] constexpr bool accumulate_digits( Integer& result, const std::string_view input ) noexcept
   {
      // Assumes input is a non-empty sequence of digits; returns false on overflow.

      for( char c : input ) {
         if( !accumulate_digit< Integer, Maximum >( result, c ) ) {
            return false;
         }
      }
      return true;
   }

   template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
   [[nodiscard]] constexpr bool convert_positive( Integer& result, const std::string_view input ) noexcept
   {
      // Assumes result == 0 and that input is a non-empty sequence of digits; returns false on overflow.

      static_assert( std::is_integral_v< Integer > );
      return accumulate_digits< Integer, Maximum >( result, input );
   }

   template< typename Signed >
   [[nodiscard]] constexpr bool convert_negative( Signed& result, const std::string_view input ) noexcept
   {
      // Assumes result == 0 and that input is a non-empty sequence of digits; returns false on overflow.

      static_assert( std::is_signed_v< Signed > );
      using Unsigned = std::make_unsigned_t< Signed >;
      constexpr Unsigned maximum = static_cast< Unsigned >( ( std::numeric_limits< Signed >::max )() ) + 1;
      Unsigned temporary = 0;
      if( accumulate_digits< Unsigned, maximum >( temporary, input ) ) {
         result = static_cast< Signed >( ~temporary ) + 1;
         return true;
      }
      return false;
   }

   template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   [[nodiscard]] constexpr bool convert_unsigned( Unsigned& result, const std::string_view input ) noexcept
   {
      // Assumes result == 0 and that input is a non-empty sequence of digits; returns false on overflow.

      static_assert( std::is_unsigned_v< Unsigned > );
      return accumulate_digits< Unsigned, Maximum >( result, input );
   }

   template< typename Signed >
   [[nodiscard]] constexpr bool convert_signed( Signed& result, const std::string_view input ) noexcept
   {
      // Assumes result == 0 and that input is an optional sign followed by a non-empty sequence of digits; returns false on overflow.

      static_assert( std::is_signed_v< Signed > );
      if( input[ 0 ] == '-' ) {
         return convert_negative< Signed >( result, std::string_view( input.data() + 1, input.size() - 1 ) );
      }
      const auto offset = static_cast< unsigned >( input[ 0 ] == '+' );
      return convert_positive< Signed >( result, std::string_view( input.data() + offset, input.size() - offset ) );
   }

   template< typename ParseInput >
   [[nodiscard]] bool match_unsigned( ParseInput& in ) noexcept( noexcept( in.empty() ) )
   {
      if( !in.empty() ) {
         const char c = in.peek_char();
         if( is_ascii_digit( c ) ) {
            in.template consume< eol_exclude_tag >( 1 );
            if( c == '0' ) {
               return in.empty() || ( !is_ascii_digit( in.peek_char() ) );
            }
            while( ( !in.empty() ) && is_ascii_digit( in.peek_char() ) ) {
               in.template consume< eol_exclude_tag >( 1 );
            }
            return true;
         }
      }
      return false;
   }

   template< typename ParseInput,
             typename Unsigned,
             Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   [[nodiscard]] bool match_and_convert_unsigned_with_maximum_throws( ParseInput& in, Unsigned& st )
   {
      // Assumes st == 0.

      if( !in.empty() ) {
         char c = in.peek_char();
         if( is_ascii_digit( c ) ) {
            if( c == '0' ) {
               in.template consume< eol_exclude_tag >( 1 );
               return in.empty() || ( !is_ascii_digit( in.peek_char() ) );
            }
            do {
               if( !accumulate_digit< Unsigned, Maximum >( st, c ) ) {
                  throw_parse_error( "integer overflow", in );
               }
               in.template consume< eol_exclude_tag >( 1 );
            } while( ( !in.empty() ) && is_ascii_digit( c = in.peek_char() ) );
            return true;
         }
      }
      return false;
   }

   template< typename ParseInput,
             typename Unsigned,
             Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   [[nodiscard]] bool match_and_convert_unsigned_with_maximum_nothrow( ParseInput& in, Unsigned& st )
   {
      // Assumes st == 0.

      if( !in.empty() ) {
         char c = in.peek_char();
         if( c == '0' ) {
            if( ( in.size( 2 ) < 2 ) || ( !is_ascii_digit( in.peek_char( 1 ) ) ) ) {
               in.template consume< eol_exclude_tag >( 1 );
               return true;
            }
            return false;
         }
         if( is_ascii_digit( c ) ) {
            unsigned b = 0;

            do {
               if( !accumulate_digit< Unsigned, Maximum >( st, c ) ) {
                  return false;
               }
               ++b;
            } while( ( !in.empty() ) && is_ascii_digit( c = in.peek_char( b ) ) );
            in.template consume< eol_exclude_tag >( b );
            return true;
         }
      }
      return false;
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
