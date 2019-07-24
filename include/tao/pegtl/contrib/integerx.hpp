// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_INTEGERX_HPP
#define TAO_PEGTL_CONTRIB_INTEGERX_HPP

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <type_traits>

#include "../ascii.hpp"
#include "../parse.hpp"
#include "../parse_error.hpp"
#include "../rules.hpp"

#include "integer.hpp"

namespace TAO_PEGTL_NAMESPACE::integerx
{
   namespace internal
   {
      struct unsigned_rule_new
         : if_then_else< one< '0' >, if_then_else< one< 'x' >, plus< xdigit >, not_at< digit > >, plus< digit > >
      {
         // Unsigned hex or dec integer, no leading zeros for dec.
      };

      struct signed_rule_new
         : seq< opt< one< '-', '+' > >, if_then_else< one< '0' >, if_then_else< one< 'x' >, plus< xdigit >, not_at< digit > >, plus< digit > > >
      {
         // Signed hex or dec integer, no leading zeros for dec.
      };

      [[nodiscard]] constexpr bool is_hex_digit( const char c ) noexcept
      {
         // We don't use std::isxdigit() because it might
         // return true for other values on MS platforms?

         return ( ( '0' <= c ) && ( c <= '9' ) ) || ( ( 'a' <= c ) && ( c <= 'f' ) ) || ( ( 'A' <= c ) && ( c <= 'F' ) );
      }

      template< typename Integer >
      [[nodiscard]] constexpr Integer convert_hex_digit( const char c ) noexcept
      {
         // Assumes that xdigit is an xdigit as per is_hex_digit().

         if( ( '0' <= c ) && ( c <= '9' ) ) {
            return Integer( c - '0' );
         }
         if( ( 'a' <= c ) && ( c <= 'f' ) ) {
            return Integer( c - 'a' + 10 );
         }
         return Integer( c - 'A' + 10 );
      }

      template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
      [[nodiscard]] constexpr bool accumulate_hex_digit( Integer& result, const char xdigit ) noexcept
      {
         // Assumes that xdigit is an xdigit as per is_hex_digit(); returns false on overflow.

         static_assert( std::is_integral_v< Integer > );

         constexpr Integer cutoff = Maximum / 16;
         constexpr Integer cutlim = Maximum % 16;

         const Integer c = convert_hex_digit< Integer >( xdigit );

         if( ( result > cutoff ) || ( ( result == cutoff ) && ( c > cutlim ) ) ) {
            return false;
         }
         result *= 16;
         result += c;
         return true;
      }

      template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
      [[nodiscard]] constexpr bool accumulate_hex_digits( Integer& result, const std::string_view input ) noexcept
      {
         // Assumes input is a non-empty sequence of hex-digits; returns false on overflow.

         for( std::size_t i = 0; i < input.size(); ++i ) {
            if( !accumulate_hex_digit< Integer, Maximum >( result, input[ i ] ) ) {
               return false;
            }
         }
         return true;
      }

      template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
      [[nodiscard]] constexpr bool convert_hex_positive( Integer& result, const std::string_view input ) noexcept
      {
         // Assumes result == 0 and that input is a non-empty sequence of hex-digits; returns false on overflow.

         static_assert( std::is_integral_v< Integer > );
         return accumulate_hex_digits< Integer, Maximum >( result, input );
      }

      template< typename Signed >
      [[nodiscard]] constexpr bool convert_hex_negative( Signed& result, const std::string_view input ) noexcept
      {
         // Assumes result == 0 and that input is a non-empty sequence of hex-digits; returns false on overflow.

         static_assert( std::is_signed_v< Signed > );
         using Unsigned = std::make_unsigned_t< Signed >;
         constexpr Unsigned maximum = static_cast< Unsigned >( ( std::numeric_limits< Signed >::max )() ) + 1;
         Unsigned temporary = 0;
         if( accumulate_hex_digits< Unsigned, maximum >( temporary, input ) ) {
            result = static_cast< Signed >( ~temporary ) + 1;
            return true;
         }
         return false;
      }

      template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
      [[nodiscard]] constexpr bool convert_hex_unsigned( Unsigned& result, const std::string_view input ) noexcept
      {
         // Assumes result == 0 and that input is a non-empty sequence of hex-digits; returns false on overflow.

         static_assert( std::is_unsigned_v< Unsigned > );
         return accumulate_hex_digits< Unsigned, Maximum >( result, input );
      }

      template< typename Signed >
      [[nodiscard]] constexpr bool convert_hex_signed( Signed& result, const std::string_view input ) noexcept
      {
         // Assumes result == 0 and that input is an optional sign followed by a "0x" and a non-empty sequence of hex-digits; returns false on overflow.

         static_assert( std::is_signed_v< Signed > );
         if( input[ 0 ] == '-' ) {
            return convert_hex_negative< Signed >( result, std::string_view( input.data() + 3, input.size() - 3 ) );
         }
         const auto offset = unsigned( input[ 0 ] == '+' ) + 2;  // The "0x" prefix has length 2.
         return convert_hex_positive< Signed >( result, std::string_view( input.data() + offset, input.size() - offset ) );
      }

   }  // namespace internal

   using unsigned_dec_action = integer::unsigned_action;

   struct unsigned_hex_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII hex-digits with "0x" prefix.

      template< typename Input, typename Unsigned >
      static auto apply( const Input& in, Unsigned& st ) -> std::enable_if_t< std::is_unsigned_v< Unsigned >, void >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         if( !internal::convert_hex_unsigned( st, in.string_view( 2 ) ) ) {
            throw parse_error( "unsigned hex integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );  // Compatibility for pre-3.0 behaviour.
      }

      template< typename Input, typename Unsigned, typename... Ts >
      static auto apply( const Input& in, std::vector< Unsigned, Ts... >& st ) -> std::enable_if_t< std::is_unsigned_v< Unsigned >, void >
      {
         Unsigned u = 0;
         apply( in, u );
         st.emplace_back( u );
      }
   };

   struct unsigned_rule
   {
      using analyze_t = internal::unsigned_rule_new::analyze_t;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.empty() ) )
      {
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new >( in );  // Does not check for any overflow.
      }
   };

   namespace internal
   {
      template< typename Rule >
      struct unsigned_action_action
         : nothing< Rule >
      {
      };

      template<>
      struct unsigned_action_action< plus< digit > >
         : unsigned_dec_action
      {
      };

      template<>
      struct unsigned_action_action< plus< xdigit > >
      {
         template< typename Input, typename Unsigned >
         static void apply( const Input& in, Unsigned& st )
         {
            // This function "only" offers basic exception safety.
            st = 0;
            if( !internal::convert_hex_unsigned( st, in.string_view() ) ) {
               throw parse_error( "unsigned hex integer overflow", in );
            }
         }
      };

   }  // namespace internal

   struct unsigned_rule_with_action
   {
      using analyze_t = internal::unsigned_rule_new::analyze_t;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< A == apply_mode::nothing, bool >
      {
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new >( in );  // Does not check for any overflow.
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Unsigned >
      [[nodiscard]] static auto match( Input& in, Unsigned& st ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_unsigned_v< Unsigned >, bool >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new, internal::unsigned_action_action >( in, st );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   template< typename Unsigned, Unsigned Maximum >
   using maximum_dec_action = integer::maximum_action< Unsigned, Maximum >;

   template< typename Unsigned, Unsigned Maximum >
   struct maximum_hex_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII hex-digits.

      static_assert( std::is_unsigned_v< Unsigned > );

      template< typename Input, typename Unsigned2 >
      static auto apply( const Input& in, Unsigned2& st ) -> std::enable_if_t< std::is_same_v< Unsigned, Unsigned2 >, void >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         if( !internal::convert_hex_unsigned< Unsigned, Maximum >( st, in.string_view( 2 ) ) ) {
            throw parse_error( "unsigned hex integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );  // Compatibility for pre-3.0 behaviour.
      }

      template< typename Input, typename Unsigned2, typename... Ts >
      static auto apply( const Input& in, std::vector< Unsigned2, Ts... >& st ) -> std::enable_if_t< std::is_same_v< Unsigned, Unsigned2 >, void >
      {
         Unsigned u = 0;
         apply( in, u );
         st.emplace_back( u );
      }
   };

   namespace internal
   {
      template< typename Rule, typename Unsigned, Unsigned Maximum >
      struct maximum_action_action
         : nothing< Rule >
      {
      };

      template< typename Unsigned, Unsigned Maximum >
      struct maximum_action_action< plus< digit >, Unsigned, Maximum >
         : maximum_dec_action< Unsigned, Maximum >
      {
      };

      template< typename Unsigned, Unsigned Maximum >
      struct maximum_action_action< plus< xdigit >, Unsigned, Maximum >
      {
         template< typename Input, typename Unsigned2 >
         static auto apply( const Input& in, Unsigned2& st ) -> std::enable_if_t< std::is_same_v< Unsigned, Unsigned2 >, void >
         {
            if( !internal::convert_hex_unsigned< Unsigned, Maximum >( st, in.string_view() ) ) {
               throw parse_error( "unsigned hex integer overflow", in );
            }
         }
      };

      template< typename Unsigned, Unsigned Maximum >
      struct maximum_action_helper
      {
         template< typename Rule >
         struct type
            : maximum_action_action< Rule, Unsigned, Maximum >
         {
         };
      };

   }  // namespace internal

   template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   struct maximum_rule
   {
      static_assert( std::is_unsigned_v< Unsigned > );

      using analyze_t = internal::unsigned_rule_new::analyze_t;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in )
      {
         Unsigned st = 0;
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new, internal::maximum_action_helper< Unsigned, Maximum >::template type >( in, st );  // Throws on overflow.
      }
   };

   template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   struct maximum_rule_with_action
   {
      static_assert( std::is_unsigned_v< Unsigned > );

      using analyze_t = internal::unsigned_rule_new::analyze_t;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) -> std::enable_if_t< A == apply_mode::nothing, bool >
      {
         Unsigned st = 0;
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new, internal::maximum_action_helper< Unsigned, Maximum >::type >( in );  // Throws on overflow.
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Unsigned2 >
      [[nodiscard]] static auto match( Input& in, Unsigned2& st ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_same_v< Unsigned, Unsigned2 >, bool >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         return TAO_PEGTL_NAMESPACE::parse< internal::unsigned_rule_new, internal::maximum_action_helper< Unsigned, Maximum >::type >( in );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   using signed_dec_action = integer::signed_action;

   struct signed_hex_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII hex-digits,
      // with optional leading sign before the "0x" prefix; when there is a
      // sign, in.size() must be >= 4.

      template< typename Input, typename Signed >
      static auto apply( const Input& in, Signed& st ) -> std::enable_if_t< std::is_signed_v< Signed >, void >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         if( !internal::convert_hex_signed( st, in.string_view() ) ) {
            throw parse_error( "signed hex integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );  // Compatibility for pre-3.0 behaviour.
      }

      template< typename Input, typename Signed, typename... Ts >
      static auto apply( const Input& in, std::vector< Signed, Ts... >& st ) -> std::enable_if_t< std::is_signed_v< Signed >, void >
      {
         Signed s = 0;
         apply( in, s );
         st.emplace_back( s );
      }
   };

   struct signed_rule
   {
      using analyze_t = internal::signed_rule_new::analyze_t;

      template< typename Input >
      [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.empty() ) )
      {
         return TAO_PEGTL_NAMESPACE::parse< internal::signed_rule_new >( in );  // Does not check for any overflow.
      }
   };

   namespace internal
   {
      template< typename Rule >
      struct signed_action_action
         : nothing< Rule >
      {
      };

      template<>
      struct signed_action_action< one< '-', '+' > >
      {
         template< typename Input, typename Signed >
         static void apply( const Input& in, bool& negative, Signed& /*unused*/ )
         {
            negative = ( in.peek_char() == '-' );  // TODO: Optimise with custom rule to prevent building marker and action_input for single char?
         }
      };

      template<>
      struct signed_action_action< plus< digit > >
      {
         template< typename Input, typename Signed >
         static void apply( const Input& in, const bool negative, Signed& st )
         {
            if( !( negative ? integer::internal::convert_negative< Signed >( st, in.string_view() ) : integer::internal::convert_positive< Signed >( st, in.string_view() ) ) ) {
               throw parse_error( "signed dec integer overflow", in );
            }
         }
      };

      template<>
      struct signed_action_action< plus< xdigit > >
      {
         template< typename Input, typename Signed >
         static void apply( const Input& in, const bool negative, Signed& st )
         {
            if( !( negative ? internal::convert_hex_negative< Signed >( st, in.string_view() ) : internal::convert_hex_positive< Signed >( st, in.string_view() ) ) ) {
               throw parse_error( "signed hex integer overflow", in );
            }
         }
      };

   }  // namespace internal

   struct signed_rule_with_action
   {
      using analyze_t = internal::signed_rule_new::analyze_t;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< A == apply_mode::nothing, bool >
      {
         return TAO_PEGTL_NAMESPACE::parse< internal::signed_rule_new >( in );  // Does not check for any overflow.
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Signed >
      [[nodiscard]] static auto match( Input& in, Signed& st ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_signed_v< Signed >, bool >
      {
         // This function "only" offers basic exception safety.
         st = 0;
         bool negative = false;                                                                                               // Superfluous initialisation.
         return TAO_PEGTL_NAMESPACE::parse< internal::signed_rule_new, internal::signed_action_action >( in, negative, st );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Signed >?
   };

}  // namespace TAO_PEGTL_NAMESPACE::integerx

#endif
