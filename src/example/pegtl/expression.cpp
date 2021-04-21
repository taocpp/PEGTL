// Copyright (c) 2021 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cerr << "Exception support required, example unavailable." << std::endl;
   return 1;
}
#else

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <variant>
#include <vector>

#include <tao/pegtl.hpp>

namespace TAO_PEGTL_NAMESPACE::expression
{
   // Expression parsing with prefix, postfix and infix operators, ternary
   // operator and a couple of other special cases supported.

   // The handling of operator precedences with left and right binding power is
   // based on https://github.com/matklad/minipratt/blob/master/src/bin/pratt.rs

   // It correctly recognises all operators with their precedence and associativity,
   // however is still very much work-in-progress regarding a lot of details...

   // TODO: Fix missing whitespace-skip before infix/postfix operators.
   // TODO: Decide whether to use must everywhere or nowhere?
   // TODO: Decide whether to suppress actions for sub-rules.
   // TODO: Finalise the event-style interface or change to fake actions or actions with ops?
   // TODO: Decide on where to use config vs. where to use grammar template parameters.
   // TODO: Choose customisation points vs. copy-n-paste customisation.
   // TODO: Eliminate use of functions from cstring?
   // TODO: Remove small_string [premature] optimisation and use std::string instead?
   // TODO: In general choose simplicity over performance in _all_ cases?
   // TODO: Constexpr-ify where possible with C++20.

   namespace internal
   {
      template< std::uint8_t N >
      struct small_string
      {
         static_assert( N > 0 );

         small_string() noexcept = default;

         explicit small_string( const char c )
            : m_size( 1 )
         {
            m_data[ 0 ] = c;
         }

         explicit small_string( const std::string_view s )
         {
            assign( s );
         }

         small_string( const char* d, const std::size_t s )
         {
            assign( d, s );
         }

         template< std::size_t S >
         small_string( const char ( &d )[ S ] )
         {
            assign( d, S - 1 );
         }

         small_string( small_string&& ) noexcept = default;
         small_string( const small_string& ) noexcept = default;

         small_string& operator=( small_string&& ) noexcept = default;
         small_string& operator=( const small_string& ) noexcept = default;

         ~small_string() = default;

         [[nodiscard]] bool empty() const noexcept
         {
            return m_size == 0;
         }

         [[nodiscard]] std::size_t size() const noexcept
         {
            return m_size;
         }

         [[nodiscard]] std::size_t free() const noexcept
         {
            return N - m_size;
         }

         [[nodiscard]] static std::size_t capacity() noexcept
         {
            return N;
         }

         [[nodiscard]] static std::size_t max_size() noexcept
         {
            return N;
         }

         [[nodiscard]] const char* data() const noexcept
         {
            return m_data;  // This is NOT '\0'-terminated!
         }

         [[nodiscard]] std::string_view string_view() const noexcept
         {
            return std::string_view( data(), size() );
         }

         [[nodiscard]] bool equals( const char c ) const noexcept
         {
            return ( size() == 1 ) && ( m_data[ 0 ] == c );
         }

         [[nodiscard]] bool equals( const std::string_view sv ) const noexcept
         {
            return sv == string_view();
         }

         void assign( const char c ) noexcept
         {
            m_data[ 0 ] = c;
            m_size = 1;
         }

         void assign( const std::string_view s )
         {
            assign( s.data(), s.size() );
         }

         void assign( const char* d, const std::size_t s )
         {
            check_size( s );
            std::memcpy( m_data, d, s );
            m_size = s;
         }

         template< std::size_t S >
         void assign( const char ( &d )[ S ] )
         {
            assign( d, S - 1 );
         }

         void unsafe_pop() noexcept
         {
            assert( m_size > 0 );
            --m_size;
         }

      private:
         char m_data[ N ];
         std::uint8_t m_size = 0;

         static void check_size( const std::size_t s )
         {
            if( s > N ) {
               throw std::logic_error( "small string overflow" );
            }
         }
      };

      template< std::uint8_t N, std::uint8_t M >
      [[nodiscard]] bool operator<( const small_string< N > l, const small_string< M > r ) noexcept
      {
         return l.string_view() < r.string_view();
      }

      template< std::uint8_t N, std::uint8_t M >
      [[nodiscard]] bool operator==( const small_string< N > l, const small_string< M > r ) noexcept
      {
         return l.string_view() == r.string_view();
      }

      template< std::uint8_t N, std::uint8_t M >
      [[nodiscard]] bool operator!=( const small_string< N > l, const small_string< M > r ) noexcept
      {
         return l.string_view() != r.string_view();
      }

      // TODO: Spaceship with C++20.

      using operator_name = small_string< 3 >;  // sizeof( operator_name ) == 4

      struct prefix_info
      {
         prefix_info( const std::string_view n, const std::uint8_t pbp ) noexcept
            : name( n ),
              prefix_binding_power( pbp )
         {
            assert( pbp );
         }

         operator_name name;

         std::uint8_t prefix_binding_power;
      };

      struct infix_postfix_info
      {
         infix_postfix_info( const std::string_view n, const std::uint8_t lbp, const std::uint8_t rbp = 0 ) noexcept
            : infix_postfix_info( n, std::string_view(), lbp, rbp )
         {}

         infix_postfix_info( const std::string_view n, const std::string_view o, const std::uint8_t lbp, const std::uint8_t rbp = 0 ) noexcept
            : name( n ),
              other( o ),
              left_binding_power( lbp ),
              right_binding_power( rbp )
         {
            if( right_binding_power ) {
               assert( std::min( left_binding_power, right_binding_power ) & 1 );
               assert( 2 * std::min( left_binding_power, right_binding_power ) + 1 == left_binding_power + right_binding_power );
            }
            assert( left_binding_power );
         }

         [[nodiscard]] bool is_infix() const noexcept
         {
            return right_binding_power != 0;
         }

         [[nodiscard]] bool is_postfix() const noexcept
         {
            return right_binding_power == 0;
         }

         operator_name name;
         operator_name other;  // Used for the ':' of the ternary operator etc.

         std::uint8_t left_binding_power;
         std::uint8_t right_binding_power;
      };

      template< typename ParseInput >
      [[nodiscard]] bool match_string_view( ParseInput& in, const std::string_view sv )
      {
         if( in.size( sv.size() ) >= sv.size() ) {
            if( std::memcmp( in.current(), sv.data(), sv.size() ) == 0 ) {
               in.bump( sv.size() );
               return true;
            }
         }
         return false;
      }

      template< typename ParseInput, typename OperatorInfo >
      [[nodiscard]] const OperatorInfo* match_prefix( ParseInput& in, const std::vector< OperatorInfo >& ops )
      {
         const std::size_t max = std::min( operator_name::capacity(), in.size( operator_name::capacity() ) );
         for( operator_name op( in.current(), max ); !op.empty(); op.unsafe_pop() ) {
            if( const auto i = std::find_if( ops.begin(), ops.end(), [ = ]( const OperatorInfo& info ) { return info.name == op; } ); i != ops.end() ) {
               in.bump( op.size() );
               return &*i;
            }
         }
         return nullptr;
      }

      template< typename ParseInput, typename OperatorInfo >
      [[nodiscard]] const OperatorInfo* match_infix_postfix( ParseInput& in, const std::vector< OperatorInfo >& ops, const std::uint8_t min )
      {
         const std::size_t max = std::min( operator_name::capacity(), in.size( operator_name::capacity() ) );
         for( operator_name op( in.current(), max ); !op.empty(); op.unsafe_pop() ) {
            if( const auto i = std::find_if( ops.begin(), ops.end(), [ = ]( const OperatorInfo& info ) { return info.name == op; } ); ( i != ops.end() ) && ( i->left_binding_power >= min ) ) {
               in.bump( op.size() );
               return &*i;
            }
         }
         return nullptr;
      }

      // TODO: constexpr-ify with C++20:

      template< typename T >
      [[nodiscard]] std::vector< T > sorted_operator_vector( const std::initializer_list< T >& t )
      {
         std::vector< T > r{ t };
         const auto less = []( const auto& l, const auto& r ) { return l.name < r.name; };
         std::sort( r.begin(), r.end(), less );
         return r;
      }

      struct operator_maps
      {
         // clang-format off
         operator_maps()
            : prefix( sorted_operator_vector( {
                  prefix_info( "!", 80 ),
                  prefix_info( "+", 80 ),
                  prefix_info( "-", 80 ),
                  prefix_info( "~", 80 ),
                  prefix_info( "*", 80 ),
                  prefix_info( "&", 80 ),
                  prefix_info( "++", 80 ),
                  prefix_info( "--", 80 )
               } ) ),
              infix_postfix( sorted_operator_vector( {
                  infix_postfix_info( "::", 99, 100 ),  // Special: Followed by identifier (or template-space-identifer, which we don't support yet).
                  infix_postfix_info( ".*", 37, 38 ),
                  infix_postfix_info( "->*", 37, 38 ),
                  infix_postfix_info( "*", 35, 36 ),
                  infix_postfix_info( "/", 35, 36 ),
                  infix_postfix_info( "%", 35, 36 ),
                  infix_postfix_info( "+", 33, 34 ),
                  infix_postfix_info( "-", 33, 34 ),
                  infix_postfix_info( "<<", 31, 32 ),
                  infix_postfix_info( ">>", 31, 32 ),
                  infix_postfix_info( "<=>", 29, 30 ),
                  infix_postfix_info( "<", 27, 28 ),
                  infix_postfix_info( "<=", 27, 28 ),
                  infix_postfix_info( ">", 27, 28 ),
                  infix_postfix_info( ">=", 27, 28 ),
                  infix_postfix_info( "==", 25, 26 ),
                  infix_postfix_info( "!=", 25, 26 ),
                  infix_postfix_info( "&", 23, 24 ),
                  infix_postfix_info( "^", 21, 22 ),
                  infix_postfix_info( "|", 19, 20 ),
                  infix_postfix_info( "&&", 17, 18 ),
                  infix_postfix_info( "||", 15, 16 ),
                  infix_postfix_info( "?", ":", 14, 13 ),  // Special: Ternary operator.
                  infix_postfix_info( "=", 12, 11 ),
                  infix_postfix_info( "+=", 12, 11 ),
                  infix_postfix_info( "-=", 12, 11 ),
                  infix_postfix_info( "*=", 12, 11 ),
                  infix_postfix_info( "/=", 12, 11 ),
                  infix_postfix_info( "%=", 12, 11 ),
                  infix_postfix_info( "<<=", 12, 11 ),
                  infix_postfix_info( ">>=", 12, 11 ),
                  infix_postfix_info( "&=", 12, 11 ),
                  infix_postfix_info( "^=", 12, 11 ),
                  infix_postfix_info( "|=", 12, 11 ),
                  // infix_postfix_info( ",", 9, 10 ),  // TODO: Enable, but forbid in function argument list.
                  infix_postfix_info( "[", "]", 90 ),  // Special: Argument list.
                  infix_postfix_info( "(", ")", 90 ),  // Special: Argument list.
                  infix_postfix_info( ".", 90 ),  // Special: Followed by identifier.
                  infix_postfix_info( "->", 90 ),  // Special: Followed by identifier.
                  infix_postfix_info( "++", 90 ),
                  infix_postfix_info( "--", 90 )
               } ) )
         {
            // These are C++20 operators with the correct associativity and relative precedence, however some are still missing:
            // TODO: Compound literal (C99), _Alignof (C11), Functional cast, sizeof, co_await, co_yield, throw, new, new[], delete, delete[], C-style casts.
         }
         // clang-format on

         const std::vector< prefix_info > prefix;
         const std::vector< infix_postfix_info > infix_postfix;
      };

      struct string_view_rule
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput >
         [[nodiscard]] static bool match( ParseInput& in, const std::string_view sv ) noexcept( noexcept( match_string_view( in, sv ) ) )
         {
            return match_string_view( in, sv );
         }
      };

      struct comment
         : seq< one< '#' >, until< eolf > >
      {};

      struct ignored
         : sor< space, comment >
      {};

      template< typename Literal, typename Identifier >
      struct expression;

      template< typename Literal, typename Identifier >
      struct bracket_expression
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename Result,
                   typename Config >
         [[nodiscard]] static bool match( ParseInput& in, Result& res, const Config& cfg, const std::uint8_t /*unused*/ )
         {
            return Control< if_must< one< '(' >, star< ignored >, expression< Literal, Identifier >, star< ignored >, one< ')' > > >::template match< A, M, Action, Control >( in, res, cfg, 0 );
         }
      };

      template< typename Literal, typename Identifier >
      struct prefix_expression
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename Result,
                   typename Config >
         [[nodiscard]] static bool match( ParseInput& in, Result& res, const Config& cfg, const std::uint8_t /*unused*/ )
         {
            if( const auto* info = match_prefix( in, cfg.prefix ) ) {
               (void)Control< must< star< ignored >, expression< Literal, Identifier > > >::template match< A, M, Action, Control >( in, res, cfg, info->prefix_binding_power );
               if constexpr( A == apply_mode::action ) {
                  res.prefix( info->name.string_view() );
               }
               return true;
            }
            return false;
         }
      };

      template< typename Literal, typename Identifier >
      struct infix_postfix_expression
      {
         template< apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename Result,
                   typename Config >
         [[nodiscard]] static bool match( ParseInput& in, Result& res, const Config& cfg, const std::uint8_t min )
         {
            if( const auto* info = match_infix_postfix( in, cfg.infix_postfix, min ) ) {
               if( info->name.equals( '?' ) ) {
                  (void)Control< must< star< ignored >, expression< Literal, Identifier > > >::template match< A, M, Action, Control >( in, res, cfg, 0 );
                  (void)Control< must< star< ignored >, string_view_rule > >::template match< A, M, Action, Control >( in, info->other.string_view() );
                  (void)Control< must< star< ignored >, expression< Literal, Identifier > > >::template match< A, M, Action, Control >( in, res, cfg, info->right_binding_power );
                  if constexpr( A == apply_mode::action ) {
                     res.ternary( info->name.string_view(), info->other.string_view() );
                  }
                  return true;
               }
               if( info->name.equals( '.' ) || info->name.equals( "::" ) || info->name.equals( "->" ) ) {
                  (void)Control< must< star< ignored >, Identifier > >::template match< A, M, Action, Control >( in, res, cfg, 0 );
                  if constexpr( A == apply_mode::action ) {
                     res.infix( info->name.string_view() );
                  }
                  return true;
               }
               if( info->name.equals( '(' ) || info->name.equals( '[' ) ) {
                  const std::size_t size = res.term_stack.size();  // TODO: Determine number of arguments without relying on res!?
                  (void)Control< must< star< ignored >, list_must< expression< Literal, Identifier >, one< ',' >, ignored > > >::template match< A, M, Action, Control >( in, res, cfg, 0 );
                  (void)Control< must< star< ignored >, string_view_rule > >::template match< A, M, Action, Control >( in, info->other.string_view() );
                  if constexpr( A == apply_mode::action ) {
                     res.call( info->name.string_view(), info->other.string_view(), res.term_stack.size() - size );
                  }
                  return true;
               }
               if( info->is_infix() ) {
                  (void)Control< must< star< ignored >, expression< Literal, Identifier > > >::template match< A, M, Action, Control >( in, res, cfg, info->right_binding_power );
                  if constexpr( A == apply_mode::action ) {
                     res.infix( info->name.string_view() );
                  }
                  return true;
               }
               if( info->is_postfix() ) {
                  if constexpr( A == apply_mode::action ) {
                     res.postfix( info->name.string_view() );
                  }
                  return true;
               }
            }
            return false;
         }
      };

      template< typename Literal, typename Identifier >
      struct first_expression
         : sor< Literal, Identifier, bracket_expression< Literal, Identifier >, prefix_expression< Literal, Identifier > >
      {};

      template< typename Literal, typename Identifier >
      struct expression
         : seq< first_expression< Literal, Identifier >, star< infix_postfix_expression< Literal, Identifier > > >
      {};

   }  // namespace internal

   template< typename Literal, typename Identifier >
   struct grammar
   {
      using rule_t = grammar;
      using subs_t = type_list< internal::expression< Literal, Identifier > >;

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename Result >
      [[nodiscard]] static bool match( ParseInput& in, Result& res )
      {
         const internal::operator_maps cfg;
         return match< A, M, Action, Control >( in, res, cfg );
      }

      template< apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename ParseInput,
                typename Result,
                typename Config >
      [[nodiscard]] static bool match( ParseInput& in, Result& res, const Config& cfg )
      {
         return Control< internal::expression< Literal, Identifier > >::template match< A, M, Action, Control >( in, res, cfg, 0 );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::expression

namespace application
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   struct term_t;

   using tuple_t = std::tuple< std::string, std::vector< term_t > >;
   using variant_t = std::variant< long, std::string, tuple_t >;

   struct term_t
   {
      explicit term_t( const long l ) noexcept
         : variant( l )
      {}

      explicit term_t( std::string&& s ) noexcept
         : variant( std::move( s ) )
      {}

      explicit term_t( variant_t&& v ) noexcept
         : variant( std::move( v ) )
      {}

      variant_t variant;
   };

   [[nodiscard]] inline std::string operator+( const char* l, const std::string_view r )
   {
      return std::string( l ) + " '" + std::string( r ) + "'";
   }

   struct result
   {
      void infix( const std::string_view op )
      {
         assert( term_stack.size() >= 2 );
         {
            variant_t tmp = tuple_t( "infix" + op, { std::move( term_stack.at( term_stack.size() - 2 ) ), std::move( term_stack.at( term_stack.size() - 1 ) ) } );
            term_stack.pop_back();
            term_stack.back().variant = std::move( tmp );
         }
         assert( string_stack.size() >= 2 );
         {
            std::string tmp = "( " + string_stack.at( string_stack.size() - 2 ) + " " + std::string( op ) + " " + string_stack.at( string_stack.size() - 1 ) + " )";
            string_stack.pop_back();
            string_stack.back() = std::move( tmp );
         }
      }

      void prefix( const std::string_view op )
      {
         assert( term_stack.size() >= 1 );
         {
            variant_t tmp = tuple_t( "prefix" + op, { std::move( term_stack.at( term_stack.size() - 1 ) ) } );
            term_stack.back().variant = std::move( tmp );
         }
         assert( string_stack.size() >= 1 );
         {
            std::string tmp = std::string( op ) + "( " + string_stack.at( string_stack.size() - 1 ) + " )";
            string_stack.back() = std::move( tmp );
         }
      }

      void postfix( const std::string_view op )
      {
         assert( term_stack.size() >= 1 );
         {
            variant_t tmp = tuple_t( "postfix" + op, { std::move( term_stack.at( term_stack.size() - 1 ) ) } );
            term_stack.back().variant = std::move( tmp );
         }
         assert( string_stack.size() >= 1 );
         {
            std::string tmp = "( " + string_stack.at( string_stack.size() - 1 ) + " )" + std::string( op );
            string_stack.back() = std::move( tmp );
         }
      }

      void ternary( const std::string_view op, const std::string_view o2 )
      {
         assert( term_stack.size() >= 2 );
         {
            variant_t tmp = tuple_t( "ternary", { std::move( term_stack.at( term_stack.size() - 3 ) ), std::move( term_stack.at( term_stack.size() - 2 ) ), std::move( term_stack.at( term_stack.size() - 1 ) ) } );
            term_stack.pop_back();
            term_stack.pop_back();
            term_stack.back().variant = std::move( tmp );
         }
         assert( string_stack.size() >= 2 );
         {
            std::string tmp = "( " + string_stack.at( string_stack.size() - 3 ) + " " + std::string( op ) + " " + string_stack.at( string_stack.size() - 2 ) + " " + std::string( o2 ) + " " + string_stack.at( string_stack.size() - 1 ) + " )";
            string_stack.pop_back();
            string_stack.pop_back();
            string_stack.back() = std::move( tmp );
         }
      }

      void call( const std::string_view op, const std::string_view o2, const std::size_t args )
      {
         assert( term_stack.size() > args );
         {
            variant_t tmp = tuple_t( "call '" + std::string( op ) + std::string( o2 ) + "'", std::vector< term_t >( term_stack.end() - args - 1, term_stack.end() ) );
            for( std::size_t i = 0; i < args; ++i ) {
               term_stack.pop_back();
            }
            term_stack.back().variant = ( std::move( tmp ) );
         }
         assert( string_stack.size() > args );
         {
            std::string tmp = *( string_stack.end() - args - 1 ) + std::string( op ) + " ";
            for( std::size_t i = 0; i < args; ++i ) {
               if( i ) {
                  tmp += ", ";
               }
               tmp += *( string_stack.end() - args + i );
            }
            tmp += " " + std::string( o2 );
            string_stack.resize( string_stack.size() - args );
            string_stack.back() = std::move( tmp );
         }
      }

      void number( const long l )
      {
         term_stack.emplace_back( l );
         string_stack.emplace_back( std::to_string( l ) );
      }

      void identifier( const std::string& id )
      {
         term_stack.emplace_back( id );
         string_stack.emplace_back( id );
      }

      std::vector< term_t > term_stack;
      std::vector< std::string > string_stack;
   };

   inline std::ostream& operator<<( std::ostream& o, const term_t& t );

   inline std::ostream& operator<<( std::ostream& o, const tuple_t& t )
   {
      o << "{ " << std::get< 0 >( t );
      for( const auto& res : std::get< 1 >( t ) ) {
         o << " " << res;
      }
      o << " }";
      return o;
   }

   inline std::ostream& operator<<( std::ostream& o, const variant_t& v )
   {
      std::visit( [ & ]( const auto& t ) { o << t; }, v );
      return o;
   }

   inline std::ostream& operator<<( std::ostream& o, const term_t& t )
   {
      o << t.variant;
      return o;
   }

   struct literal
      : pegtl::plus< pegtl::digit >
   {};

   struct grammar
      : pegtl::must< pegtl::expression::grammar< literal, pegtl::identifier >, pegtl::eof >
   {};

   template< typename Rule >
   struct action
      : pegtl::nothing< Rule >
   {};

   template<>
   struct action< literal >
   {
      template< typename Input, typename... States >
      static void apply( const Input& in, result& res, States&&... /*unused*/ )
      {
         res.number( std::stoll( in.string() ) );
      }
   };

   template<>
   struct action< pegtl::identifier >
   {
      template< typename Input, typename... States >
      static void apply( const Input& in, result& res, States&&... /*unused*/ )
      {
         res.identifier( in.string() );
      }
   };

}  // namespace application

int main( int argc, char** argv )
{
   // if( TAO_PEGTL_NAMESPACE::analyze< application::grammar >() != 0 ) {
   //    return 1;
   // }
   for( int i = 1; i < argc; ++i ) {
      TAO_PEGTL_NAMESPACE::argv_input in( argv, i );
      try {
         application::result res;
         TAO_PEGTL_NAMESPACE::parse< application::grammar, application::action >( in, res );
         std::cout << "Input: " << argv[ i ] << std::endl;
         assert( res.term_stack.size() == 1 );
         assert( res.string_stack.size() == 1 );
         std::cout << "Result: " << res.string_stack.at( 0 ) << std::endl;
         std::cout << "Result: " << res.term_stack.at( 0 ) << std::endl;
      }
      catch( const TAO_PEGTL_NAMESPACE::parse_error& e ) {
         const auto p = e.positions().front();
         std::cerr << e.what() << '\n'
                   << in.line_at( p ) << '\n'
                   << std::setw( p.column ) << '^' << '\n';
      }
   }
   return 0;
}

#endif
