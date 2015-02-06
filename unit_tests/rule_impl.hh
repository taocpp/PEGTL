// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_TESTING_UNIT_TEST_RULE_IMPL_HH
#define PEGTL_TESTING_UNIT_TEST_RULE_IMPL_HH

#include <cstddef>
#include <cassert>
#include <iostream>

#include <pegtl.hh>

namespace pegtl
{
   std::size_t total;
   std::size_t fails;

   template< typename Rule, error_mode M >
   int verify_help( input & i )
   {
      try {
         if ( internal::rule_match_help< Rule, M, nothing, normal >( i ) ) {
            return 1;
         }
         return 0;
      }
      catch ( const std::exception & ) {
         return -1;
      }
      catch ( ... ) {
         return -2;
      }
   }

   template< error_mode M >
   int result_help( const int result )
   {
      switch ( result ) {
         case 1:
            return 1;
         case 0:
            return ( M == error_mode::RETURN ) ? 0 : -1;
         case -1:
            return -1;
      }
      assert( 0 );
   }

   template< typename Rule, error_mode M >
   void verify_impl( const std::string & data, const int result, const std::size_t remain, const std::size_t line )
   {
      ++total;
      pegtl::input i( line, 0, data.data(), data.data() + data.size(), __FILE__ );
      const int h = result_help< M >( result );
      const int r = verify_help< Rule, M >( i );
      if ( ( r == h ) && ( ( r == -1 ) || ( i.size() == remain ) ) ) {
         return;
      }
      std::cerr << "pegtl: verify rule [ " << internal::demangle< Rule >() << " ] data [ '" << data << "' ] result received/expected [ " << r << " / " << h << " ] remain received/expected [ " << i.size() << " / " << remain << " ] in line [ " << line << " ]" << std::endl;
      ++fails;
   }

   template< typename Rule >
   struct dummy_action
   {
      template< typename Input >
      static void apply( const Input & )
      { }
   };

   template< typename Rule >
   struct dummy_control
   {
      template< typename Input, typename ... States >
      static void start( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void success( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void failure( const Input &, States && ... )
      { }

      template< typename Input, typename ... States >
      static void raise( const Input & in, States && ... )
      {
         PEGTL_THROW_PARSE_ERROR( Rule, in );
      }
   };

   template< typename Rule >
   void verify( const std::string & data, const int result, const std::size_t remain, const std::size_t line )
   {
      verify_impl< Rule, error_mode::THROW >( data, result, remain, line );
      verify_impl< Rule, error_mode::RETURN >( data, result, remain, line );
      verify_impl< disable< Rule >, error_mode::THROW >( data, result, remain, line );
      verify_impl< disable< Rule >, error_mode::RETURN >( data, result, remain, line );
      verify_impl< enable< dummy_action, Rule >, error_mode::THROW >( data, result, remain, line );
      verify_impl< enable< dummy_action, Rule >, error_mode::RETURN >( data, result, remain, line );
      verify_impl< control< dummy_control, Rule >, error_mode::THROW >( data, result, remain, line );
      verify_impl< control< dummy_control, Rule >, error_mode::RETURN >( data, result, remain, line );
   }

   template< typename Rule >
   void verify_char( const char data, const bool result, const std::size_t line )
   {
      verify< Rule >( std::string( std::size_t( 1 ), data ), result, ! result, line );
   }

} // pegtl

#endif
