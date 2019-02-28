// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <cassert>
#include <sstream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/change_action.hpp>
#include <tao/pegtl/contrib/change_action_and_state.hpp>
#include <tao/pegtl/contrib/change_state.hpp>
#include <tao/pegtl/contrib/json.hpp>

#include "json_classes.hpp"
#include "json_errors.hpp"
#include "json_unescape.hpp"

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

namespace examples
{
   // Basic state class that stores the result of a JSON parsing run -- a single JSON object.

   struct result_state
   {
      result_state() = default;
      result_state( const result_state& ) = delete;
      result_state( result_state&& ) = delete;

      ~result_state() = default;

      void operator=( const result_state& ) = delete;
      void operator=( result_state&& ) = delete;

      std::shared_ptr< json_base > result;
   };

   // Action class for the simple cases...

   template< typename Rule >
   struct value_action
      : unescape_action< Rule >
   {};

   struct string_state
      : public unescape_state_base
   {
      template< typename Input, typename... States >
      explicit string_state( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
      {
      }

      template< typename Input >
      void success( const Input& /*unused*/, result_state& result )
      {
         result.result = std::make_shared< string_json >( std::move( unescaped ) );
      }
   };

   template<>
   struct value_action< pegtl::json::null >
   {
      static void apply0( result_state& result )
      {
         result.result = std::make_shared< null_json >();
      }
   };

   template<>
   struct value_action< pegtl::json::true_ >
   {
      static void apply0( result_state& result )
      {
         result.result = std::make_shared< boolean_json >( true );
      }
   };

   template<>
   struct value_action< pegtl::json::false_ >
   {
      static void apply0( result_state& result )
      {
         result.result = std::make_shared< boolean_json >( false );
      }
   };

   template<>
   struct value_action< pegtl::json::number >
   {
      template< typename Input >
      static void apply( const Input& in, result_state& result )
      {
         std::stringstream ss;
         ss << in.string();
         long double v;
         ss >> v;  // NOTE: not quite correct for JSON but we'll use it for this simple example.
         result.result = std::make_shared< number_json >( v );
      }
   };

   // State and action classes to accumulate the data for a JSON array.

   struct array_state
      : public result_state
   {
      std::shared_ptr< array_json > array = std::make_shared< array_json >();

      template< typename Input, typename... States >
      explicit array_state( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
      {}

      void push_back()
      {
         array->data.push_back( std::move( result ) );
         result.reset();
      }

      template< typename Input >
      void success( const Input& /*unused*/, result_state& in_result )
      {
         if( this->result ) {
            push_back();
         }
         in_result.result = array;
      }
   };

   template< typename Rule >
   struct array_action
   {};

   template<>
   struct array_action< pegtl::json::value_separator >
   {
      static void apply0( array_state& result )
      {
         result.push_back();
      }
   };

   // State and action classes to accumulate the data for a JSON object.

   struct object_state
      : public result_state
   {
      std::string unescaped;
      std::shared_ptr< object_json > object = std::make_shared< object_json >();

      template< typename Input, typename... States >
      explicit object_state( const Input& /*unused*/, States&&... /*unused*/ ) noexcept
      {
      }

      void insert()
      {
         object->data.insert( std::make_pair( std::move( unescaped ), std::move( result ) ) );
         unescaped.clear();
         result.reset();
      }

      template< typename Input >
      void success( const Input& /*unused*/, result_state& in_result )
      {
         if( this->result ) {
            insert();
         }
         in_result.result = object;
      }
   };

   template< typename Rule >
   struct object_action
      : unescape_action< Rule >
   {};

   template<>
   struct object_action< pegtl::json::value_separator >
   {
      static void apply0( object_state& result )
      {
         result.insert();
      }
   };

   // Put together an action class that changes the actions and states as required.

   // clang-format off
   template< typename Rule > struct action {};
   template<> struct action< pegtl::json::value > : pegtl::change_action< value_action > {};

   template<> struct value_action< pegtl::json::string::content > : pegtl::change_state< string_state > {};
   template<> struct value_action< pegtl::json::array::content > : pegtl::change_action_and_state< array_action, array_state > {};
   template<> struct value_action< pegtl::json::object::content > : pegtl::change_action_and_state< object_action, object_state > {};

   struct grammar : pegtl::must< pegtl::json::text, pegtl::eof > {};
   // clang-format on

}  // namespace examples

int main( int argc, char** argv )
{
   for( int i = 1; i < argc; ++i ) {
      examples::result_state result;
      pegtl::file_input in( argv[ i ] );
      pegtl::parse< examples::grammar, examples::action, examples::errors >( in, result );
      assert( result.result );
      std::cout << result.result << std::endl;
   }
   return 0;
}
