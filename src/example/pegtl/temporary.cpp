// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE::integer
{
   namespace internal
   {
      struct unsigned_rule_old
         : plus< digit >
      {
      };

      struct unsigned_rule_new
         : if_then_else< one< '0' >, not_at< digit >, plus< digit > >
      {
      };

      struct signed_rule_old
         : seq< opt< one< '-', '+' > >, plus< digit > >
      {
      };

      struct signed_rule_new
         : seq< opt< one< '-', '+' > >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > >
      {
      };

      struct signed_rule_bis
         : seq< opt< one< '-' > >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > >
      {
      };

      struct signed_rule_ter
         : seq< one< '-', '+' >, if_then_else< one< '0' >, not_at< digit >, plus< digit > > >
      {
      };

      [[nodiscard]] constexpr bool is_digit( const char c ) noexcept
      {
         // We don't use std::isdigit() because it might
         // return true for other values on MS platforms.

         return ( '0' <= c ) && ( c <= '9' );
      }

      template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
      [[nodiscard]] constexpr bool accumulate_digit( Integer& result, const char digit ) noexcept
      {
         // Assumes that digit is a digit as per is_digit(); returns false on overflow.

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
         // Assumes that input is a non-empty sequence of digits; returns false on overflow.

         result = input[ 0 ] - '0';
         for( std::size_t i = 1; i < input.size(); ++i ) {
            if( !accumulate_digit< Integer, Maximum >( result, input[ i ] ) ) {
               return false;
            }
         }
         return true;
      }

      template< typename Integer, Integer Maximum = ( std::numeric_limits< Integer >::max )() >
      [[nodiscard]] constexpr bool convert_positive( Integer& result, const std::string_view input ) noexcept
      {
         // Assumes that input is a non-empty sequence of digits; returns false on overflow.

         static_assert( std::is_integral_v< Integer > );
         return accumulate_digits< Integer, Maximum >( result, input );
      }

      template< typename Signed >
      [[nodiscard]] constexpr bool convert_negative( Signed& result, const std::string_view input ) noexcept
      {
         // Assumes that input is a non-empty sequence of digits; returns false on overflow.

         static_assert( std::is_signed_v< Signed > );
         using Unsigned = std::make_unsigned_t< Signed >;
         constexpr Unsigned maximum = static_cast< Unsigned >( ( std::numeric_limits< Signed >::max )() ) + 1;
         Unsigned temporary;
         if( accumulate_digits< Unsigned, maximum >( temporary, input ) ) {
            result = static_cast< Signed >( ~temporary ) + 1;
            return true;
         }
         return false;
      }

      template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
      [[nodiscard]] constexpr bool convert_unsigned( Unsigned& result, const std::string_view input ) noexcept
      {
         // Assumes that input is a non-empty sequence of digits; returns false on overflow.

         static_assert( std::is_unsigned_v< Unsigned > );
         return accumulate_digits< Unsigned, Maximum >( result, input );
      }

      template< typename Signed >
      [[nodiscard]] constexpr bool convert_signed( Signed& result, const std::string_view input ) noexcept
      {
         // Assumes that input is an optional sign followed by a non-empty sequence of digits; returns false on overflow.

         static_assert( std::is_signed_v< Signed > );
         if( input[ 0 ] == '-' ) {
            return convert_negative< Signed >( result, std::string_view( input.data() + 1, input.size() - 1 ) );
         }
         const auto offset = unsigned( input[ 0 ] == '+' );
         return convert_positive< Signed >( result, std::string_view( input.data() + offset, input.size() - offset ) );
      }

      template< typename Input >
      [[nodiscard]] bool match_unsigned( Input& in ) noexcept( noexcept( in.empty() ) )
      {
         if( !in.empty() ) {
            const char c = in.peek_char();
            if( internal::is_digit( c ) ) {
               in.bump_in_this_line();
               if( c == '0' ) {
                  return in.empty() || !is_digit( in.peek_char() );  // Or throw exception?
               }
               while( !in.empty() && is_digit( in.peek_char() ) ) {
                  in.bump_in_this_line();
               }
               return true;
            }
         }
         return false;
      }

      template< typename Input,
                typename Unsigned,
                Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
      [[nodiscard]] bool match_and_convert_unsigned_with_maximum( Input& in, Unsigned& st )
      {
         if( !in.empty() ) {
            char c = in.peek_char();
            if( internal::is_digit( c ) ) {
               st = c - '0';
               in.bump_in_this_line();
               if( c == '0' ) {
                  return in.empty() || is_digit( in.peek_char() );  // Or throw exception?
               }
               while( !in.empty() && is_digit( c = in.peek_char() ) ) {
                  if( !accumulate_digit< Unsigned, Maximum >( st, c ) ) {
                     throw parse_error( "integer overflow", in );  // Should be fine for most applications.
                  }
                  in.bump_in_this_line();
               }
               return true;
            }
         }
         return false;
      }

   }  // namespace internal

   struct unsigned_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII digits.

      template< typename Input, typename Unsigned >
      static auto apply( const Input& in, Unsigned& st ) -> std::enable_if_t< std::is_integral_v< Unsigned >, void >
      {
         if( !internal::convert_unsigned( st, in.string_view() ) ) {
            throw parse_error( "unsigned integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );
      }

      template< typename Input, typename Unsigned, typename... Ts >
      static auto apply( const Input& in, std::vector< Unsigned, Ts... >& st ) -> std::enable_if_t< std::is_integral_v< Unsigned >, void >
      {
         Unsigned u;
         apply( in, u );
         st.emplace_back( u );
      }
   };

   struct unsigned_rule
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
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< ( A == apply_mode::nothing ) || ( sizeof...( States ) == 0 ), bool >
      {
         return internal::match_unsigned( in );  // Does not check for any overflow.
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
         return internal::match_and_convert_unsigned_with_maximum( in, st );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   template< typename Unsigned, Unsigned Maximum >
   struct maximum_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII digits.

      static_assert( std::is_unsigned_v< Unsigned > );

      template< typename Input >
      static auto apply( const Input& in, Unsigned& st ) -> std::enable_if_t< std::is_integral_v< Unsigned >, void >
      {
         if( !internal::convert_unsigned< Unsigned, Maximum >( st, in.string_view() ) ) {
            throw parse_error( "unsigned integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );
      }

      template< typename Input, typename... Ts >
      static auto apply( const Input& in, std::vector< Unsigned, Ts... >& st ) -> std::enable_if_t< std::is_integral_v< Unsigned >, void >
      {
         Unsigned u;
         apply( in, u );
         st.emplace_back( u );
      }
   };

   template< typename Unsigned, Unsigned Maximum = ( std::numeric_limits< Unsigned >::max )() >
   struct maximum_rule
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
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) -> std::enable_if_t< ( A == apply_mode::nothing ) || ( sizeof...( States ) == 0 ), bool >
      {
         Unsigned st = 0;  // TODO: Remove initialisation if we can shut up all compilers.
         return internal::match_and_convert_unsigned_with_maximum< in, Unsigned, Maximum >( in, st );  // Throws on overflow.
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Unsigned2 >
      [[nodiscard]] static auto match( Input& in, Unsigned& st ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_same_v< Unsigned, Unsigned2 >, bool >
      {
         return internal::match_and_convert_unsigned_with_maximum< in, Unsigned, Maximum >( in, st );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   struct signed_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII digits,
      // with optional leading sign; with sign, in.size() must be >= 2.

      template< typename Input, typename Signed >
      static auto apply( const Input& in, Signed& st ) -> std::enable_if_t< std::is_integral_v< Signed >, void >
      {
         if( !internal::convert_signed( st, in.string_view() ) ) {
            throw parse_error( "signed integer overflow", in );
         }
      }

      template< typename Input, typename State >
      static auto apply( const Input& in, State& st ) -> std::enable_if_t< std::is_class_v< State >, void >
      {
         apply( in, st.converted );
      }

      template< typename Input, typename Signed, typename... Ts >
      static auto apply( const Input& in, std::vector< Signed, Ts... >& st ) -> std::enable_if_t< std::is_integral_v< Signed >, void >
      {
         Signed s;
         apply( in, s );
         st.emplace_back( s );
      }
   };

   struct signed_rule
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
      [[nodiscard]] static auto match( Input& in, States&&... /*unused*/ ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< ( A == apply_mode::nothing ) || ( sizeof...( States ) == 0 ), bool >
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
         return TAO_PEGTL_NAMESPACE::parse< internal::signed_rule_new, signed_action >( in, st );  // Throws on overflow.
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Signed >?
   };

}  // namespace TAO_PEGTL_NAMESPACE::integer

int main( int /*unused*/, char** /*unused*/ )
{
   return 0;
}
