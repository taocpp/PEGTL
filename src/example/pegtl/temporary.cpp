// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>

namespace tao::pegtl::integer
{
   namespace internal
   {
      constexpr bool isdigit( const char c ) noexcept
      {
         return ( '0' <= c ) && ( c <= '9' );  // Make sure we are not affected by non-standard MS behaviour.
      }

      template< typename I, I Limit >
      struct integer_converter
      {
         integer_converter()
            : out( 0 )
         {
         }

         explicit integer_converter( const I c )
            : out( c - '0' )
         {
         }

         I out = 0;

         static constexpr I cutoff = Limit / 10;
         static constexpr I cutlim = Limit % 10;

         bool feed( const I c )
         {
            c -= '0';

            if( ( out > cutoff ) || ( ( out == cutoff ) && ( c > cutlim ) ) ) {
               return false;
            }
            out *= 10;
            out += c;
            return true;
         }
      };

      template< typename I, I Limit, typename Input >
      [[nodiscard]] I actual_convert( const Input& in, std::size_t index )
      {
         integer_converter< I, Limit > conv( in.peek_char( index ) );
         while( ++index < in.size() ) {
            if( !conv.feed( in.peek_char( index ) ) ) {
               throw parse_error( "integer out of range", in );
            }
         }
         return conv.out;
      }

      template< typename I, typename Input >
      [[nodiscard]] I convert_positive( const Input& in, std::size_t index )
      {
         static constexpr I limit = ( std::numeric_limits< I >::max )();
         return actual_convert< I, limit >( in, index );
      }

      template< typename I, typename Input >
      [[nodiscard]] I convert_negative( const Input& in, std::size_t index )
      {
         using U = std::make_unsigned_t< I >;
         static constexpr U limit = static_cast< U >( ( std::numeric_limits< I >::max )() ) + 1;
         return static_cast< I >( ~actual_convert< U, limit >( in, index ) ) + 1;
      }

      template< typename Input >
      [[nodiscard]] bool match_unsigned( Input& in ) noexcept( noexcept( in.empty() ) )
      {
         if( !in.empty() ) {
            char c = in.peek_char();
            if( internal::isdigit( c ) ) {
               in.bump_in_this_line();
               if( c == '0' ) {
                  return in.empty() || internal::isdigit( in.peek_char() );  // Or throw exception?
               }
               while( !in.empty() && internal::isdigit( in.peek_char() ) ) {
                  in.bump_in_this_line();
               }
               return true;
            }
         }
         return false;
      }

      template< typename Input,
                typename Unsigned,
                Unsigned Maximum >
      [[nodiscard]] bool match_and_convert_unsigned_with_maximum( Input& in, Unsigned& st ) noexcept( noexcept( in.empty() ) )
      {
         if( !in.empty() ) {
            char c = in.peek_char();
            if( internal::isdigit( c ) ) {
               in.bump_in_this_line();
               if( c == '0' ) {
                  st = 0;
                  return in.empty() || internal::isdigit( in.peek_char() );  // Or throw exception?
               }
               internal::integer_converter< Unsigned, Maximum > conv( c );
               while( !in.empty() && internal::isdigit( c = in.peek_char() ) ) {
                  if( !conv.feed( c ) ) {
                     throw parse_error( "integer out of range", in );
                  }
                  in.bump_in_this_line();
               }
               st = conv.out;
               return true;
            }
         }
         return false;
      }

   }  // namespace internal

   struct old_unsigned_rule
      : plus< digit >
   {
   };

   struct new_unsigned_rule
      : if_then_else< one< '0' >, not_at< digit >, plus< digit > >
   {
   };

   struct unsigned_rule
   {
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
         return internal::match_unsigned( in );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Unsigned >
      [[nodiscard]] static auto match( Input& in, Unsigned& st ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_unsigned_v< Unsigned >, bool >
      {
         static constexpr Unsigned maximum = ( std::numeric_limits< Unsigned >::max )();
         return internal::match_and_convert_unsigned_with_maximum< in, Unsigned, maximum >( in, st );
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   template< typename Unsigned, Unsigned Maximum >
   struct maximum_rule
   {
      static_assert( std::is_unsigned_v< Unsigned > );

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
         Unsigned st = 0;  // TODO: Remove initialisation if we can shut up all compilers.
         return internal::match_and_convert_unsigned_with_maximum< in, Unsigned, Maximum >( in, st );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename Unsigned2 >
      [[nodiscard]] static auto match( Input& in, Unsigned& st ) noexcept( noexcept( in.empty() ) ) -> std::enable_if_t< ( A == apply_mode::action ) && std::is_same_v< Unsigned, Unsigned2 >, bool >
      {
         return internal::match_and_convert_unsigned_with_maximum< in, Unsigned, Maximum >( in, st );
      }

      // TODO: Overload for st.converted?
      // TODO: Overload for std::vector< Unsigned >?
   };

   struct unsigned_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII digits.

      template< typename Input, typename Unsigned >
      static auto apply( const Input& in, Unsigned& st ) -> std::enable_if_t< std::is_integral_v< Unsigned >, void >
      {
         static_assert( std::is_integral_v< Unsigned > );
         static_assert( std::is_unsigned_v< Unsigned > );
         st = internal::convert_positive< Unsigned >( in, 0 );
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

   struct signed_rule
      : seq< opt< one< '+', '-' > >, plus< digit > >
   {
   };

   struct signed_action
   {
      // Assumes that 'in' contains a non-empty sequence of ASCII digits,
      // with optional leading sign; with sign, in.size() must be >= 2.

      template< typename Input, typename Signed >
      static auto apply( const Input& in, Signed& st ) -> std::enable_if_t< std::is_integral_v< Signed >, void >
      {
         static_assert( std::is_integral_v< Signed > );
         static_assert( std::is_signed_v< Signed > );
         const auto c = in.peek_char();
         if( c == '-' ) {
            st = internal::convert_negative< Signed >( in, 1 );
         }
         else {
            st = internal::convert_positive< Signed >( in, std::size_t( c == '+' ) );
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

}  // namespace tao::pegtl::integer

int main( int /*unused*/, char** /*unused*/ )
{
   return 0;
}
