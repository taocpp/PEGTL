// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include <iostream>
#include <string_view>
#include <vector>

#include "test.hpp"
#include "test_utility.hpp"

#include <tao/pegtl/control/state_control.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   struct test_entry
   {
      std::string_view rule;
      std::string_view func;
      std::size_t b;
   };

   std::ostream& operator<<( std::ostream& os, const test_entry& te )
   {
      os << te.rule << " " << te.func << " " << te.b;
      return os;
   }

   [[nodiscard]] inline bool operator==( const test_entry& l, const test_entry& r ) noexcept
   {
      return ( l.rule == r.rule ) && ( l.func == r.func ) && ( l.b == r.b );
   }

   template< bool E >
   struct test_state
   {
      std::vector< test_entry > trace;

      template< typename Rule >
      static constexpr bool enable = E;

      template< typename Rule, typename Input, typename... States >
      void start( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "start", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void success( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "success", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void failure( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "failure", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void raise( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "raise", ++b } );
      }

      template< typename Rule, typename Ambient, typename Input, typename... States >
      void raise_nested( const Ambient& /*unused*/, const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "raise_nested", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void unwind( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "unwind", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void apply( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "apply", ++b } );
      }

      template< typename Rule, typename Input, typename... States >
      void apply0( const Input& /*unused*/, const int a, std::size_t& b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         trace.push_back( { demangle< Rule >(), "apply0", ++b } );
      }
   };

   struct test_grammar : must< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > >, eof >
   {};

   template< typename Rule >
   struct test_action
      : nothing< Rule >
   {};

   bool test_action_one = false;

   template<>
   struct test_action< one< 'b' > >
   {
      static void apply0( const int a, const std::size_t b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         if( test_action_one ) {
            TAO_PEGTL_TEST_ASSERT( b == 0 );
            test_action_one = false;
         }
         else {
            TAO_PEGTL_TEST_ASSERT( b == 9 );
            test_action_one = true;
         }
      }
   };

   bool test_action_two = false;

   template<>
   struct test_action< two< 'b' > >
   {
      template< typename ActionInput >
      static void apply( const ActionInput& /*unused*/, const int a, const std::size_t b )
      {
         TAO_PEGTL_TEST_ASSERT( a == -1 );
         if( test_action_two ) {
            TAO_PEGTL_TEST_ASSERT( b == 0 );
            test_action_two = false;
         }
         else {
            TAO_PEGTL_TEST_ASSERT( b == 19 );
            test_action_two = true;
         }
      }
   };

   struct test_nested : not_at< try_catch_return_false< try_catch_raise_nested< must< one< 'x' > > > > >
   {};

   void unit_test()
   {
      // must< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > >, eof >
      {
         test_state< true > st;
         text_view_input< scan::lf > in( "bb" );
         std::size_t b = 0;
         const bool result = parse< test_grammar, test_action, state_control< normal >::type >( in, -1, b, st );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( test_action_one );
         TAO_PEGTL_TEST_ASSERT( test_action_two );
         TAO_PEGTL_TEST_ASSERT( b == st.trace.size() );
         b = 0;
         std::size_t i = 0;
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< test_grammar >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< internal::must< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'a' > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'a' > >(), "failure", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< seq< one< 'b' >, must< one< 'c' > > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'b' > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'b' > >(), "apply0", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'b' > >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< must< one< 'c' > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'c' > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'c' > >(), "failure", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'c' > >(), "raise", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< must< one< 'c' > > >(), "unwind", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< seq< one< 'b' >, must< one< 'c' > > > >(), "unwind", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > > >(), "failure", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< two< 'b' > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< two< 'b' > >(), "apply", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< two< 'b' > >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > > >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< internal::must< sor< one< 'a' >, try_catch_return_false< seq< one< 'b' >, must< one< 'c' > > > >, two< 'b' > > > >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< internal::must< eof > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< eof >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< eof >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< internal::must< eof > >(), "success", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< test_grammar >(), "success", ++b } );

         TAO_PEGTL_TEST_ASSERT( i == st.trace.size() );
         TAO_PEGTL_TEST_ASSERT( b == st.trace.size() );
      }
      {
         test_state< false > st;
         text_view_input< scan::lf > in( "bb" );
         std::size_t b = 0;
         const bool result = parse< test_grammar, test_action, state_control< normal >::type >( in, -1, b, st );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( !test_action_one );
         TAO_PEGTL_TEST_ASSERT( !test_action_two );
         TAO_PEGTL_TEST_ASSERT( st.trace.empty() );
         TAO_PEGTL_TEST_ASSERT( b == 0 );
      }
      // not_at< try_catch_return_false< try_catch_raise_nested< must< one< 'x' > > > > >
      {
         test_state< true > st;
         text_view_input< scan::lf > in( "a" );
         std::size_t b = 0;
         const bool result = parse< test_nested, nothing, state_control< normal >::type >( in, -1, b, st );
         TAO_PEGTL_TEST_ASSERT( result );
         TAO_PEGTL_TEST_ASSERT( b == st.trace.size() );
         b = 0;
         std::size_t i = 0;
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< test_nested >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_return_false< try_catch_raise_nested< must< one< 'x' > > > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_raise_nested< must< one< 'x' > > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< must< one< 'x' > > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'x' > >(), "start", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'x' > >(), "failure", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< one< 'x' > >(), "raise", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< must< one< 'x' > > >(), "unwind", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< must< one< 'x' > > >(), "raise_nested", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_raise_nested< must< one< 'x' > > > >(), "unwind", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< try_catch_return_false< try_catch_raise_nested< must< one< 'x' > > > > >(), "failure", ++b } );
         TAO_PEGTL_TEST_ASSERT( st.trace[ i++ ] == test_entry{ demangle< test_nested >(), "success", ++b } );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
