// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <cassert>

#include <pegtl.hh>
#include <pegtl/contrib/json.hh>
#include <pegtl/contrib/unescape.hh>

#include "json_errors.hh"
#include "json_classes.hh"

namespace examples
{
   // Action class for relevant parsing events (SAX-style).
   // Calls hooks on the first state argument as callbacks.

   template< typename Rule > struct json_action : pegtl::nothing< Rule > {};

   template<>
   struct json_action< pegtl::json::null >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.null_element();
      }
   };

   template<>
   struct json_action< pegtl::json::true_ >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.boolean_element( true );
      }
   };

   template<>
   struct json_action< pegtl::json::false_ >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.boolean_element( false );
      }
   };

   template<>
   struct json_action< pegtl::json::number >
   {
      template< typename State >
      static void apply( const pegtl::input & in, State & state )
      {
         state.number_element( std::stod( in.string() ) );  // NOTE: stod() is not quite correct for JSON but we'll use it for this example.
      }
   };

   // The approach to unescaping strings chosen here redundantly parses the
   // string again, the second time with the correct actions and state.
   // Our preferred way is to temporarily change the state as in the other
   // JSON example, however we wanted to show how unescaping can be done
   // without state changing, albeit with a nested parsing run.

   template< typename Rule > struct string_action : pegtl::nothing< Rule > {};

   template<> struct string_action< pegtl::json::unicode > : pegtl::unescape::unescape_u {};
   template<> struct string_action< pegtl::json::escaped_char > : pegtl::unescape::unescape_c< pegtl::json::escaped_char, '"', '\\', '/', '\b', '\f', '\n', '\r', '\t' > {};
   template<> struct string_action< pegtl::json::unescaped > : pegtl::unescape::append_all {};

   struct string_state
   {
      std::string unescaped;
   };

   template<>
   struct json_action< pegtl::json::string_content >
   {
      template< typename State >
      static void apply( const pegtl::input & in, State & state )
      {
         assert( ! in.empty() );
         string_state s;
         pegtl::parse_nested< pegtl::json::string_content, string_action >( in, in.string(), "JSON string", s );
         state.string_element( s.unescaped );
      }
   };

   template<>
   struct json_action< pegtl::json::begin_array >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.begin_array();
      }
   };

   template<>
   struct json_action< pegtl::json::end_array >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
            state.end_array();
      }
   };

   template<>
   struct json_action< pegtl::json::begin_object >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.begin_object();
      }
   };

   template<>
   struct json_action< pegtl::json::end_object >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.end_object();
      }
   };

   template<>
   struct json_action< pegtl::json::value_separator >
   {
      template< typename State >
      static void apply( const pegtl::input &, State & state )
      {
         state.value_separator();
      }
   };

   // Parser state class that builds a data structure from
   // the action's parsing events; requires this enum.

   enum json_mode : bool
   {
      ARRAY,
      OBJECT
   };

   // Compared to the other example that builds the same data
   // structure, everything is more concentrated into the single
   // state class that manually has to keep track of the nesting
   // of the JSON objects and arrays -- something that is handled
   // on the C++ stack by use of the change_state etc. control
   // classes in the other example.

   class json_state
   {
   public:
      json_state()
            : m_next_string_is_name( false )
      { }

      void null_element()
      {
         m_result = std::make_shared< null_json >();
      }

      void boolean_element( const bool data )
      {
         m_result = std::make_shared< boolean_json >( data );
      }

      void string_element( const std::string & data )
      {
         if ( m_next_string_is_name ) {
            m_name_stack.push_back( data );
            m_next_string_is_name = false;
         }
         else {
            m_result = std::make_shared< string_json >( data );
         }
      }

      void number_element( const double data )
      {
         m_result = std::make_shared< number_json >( data );
      }

      void begin_array()
      {
         assert( ! m_result );
         m_mode_stack.push_back( json_mode::ARRAY );
         m_array_stack.push_back( std::make_shared< array_json >() );
      }

      void array_separator()
      {
         assert( ! m_array_stack.empty() );
         assert( m_result );
         m_array_stack.back()->data.push_back( std::move( m_result ) );
         m_result.reset();
      }

      void end_array()
      {
         assert( ! m_mode_stack.empty() );
         assert( ! m_array_stack.empty() );
         if ( m_result ) {
            array_separator();
         }
         m_result = m_array_stack.back();
         m_array_stack.pop_back();
         m_mode_stack.pop_back();
      }

      void begin_object()
      {
         assert( ! m_result );
         m_next_string_is_name = true;
         m_mode_stack.push_back( json_mode::OBJECT );
         m_object_stack.push_back( std::make_shared< object_json >() );
      }

      void object_separator()
      {
         assert( ! m_name_stack.empty() );
         assert( ! m_object_stack.empty() );
         assert( m_result );
         m_object_stack.back()->data.insert( std::pair< std::string, std::shared_ptr< json_base > >( std::move( m_name_stack.back() ), std::move( m_result ) ) );
         m_name_stack.pop_back();
         m_result.reset();
         m_next_string_is_name = true;
      }

      void end_object()
      {
         assert( ! m_mode_stack.empty() );
         assert( ! m_object_stack.empty() );
         if ( m_result ) {
            object_separator();
         }
         m_result = m_object_stack.back();
         m_object_stack.pop_back();
         m_mode_stack.pop_back();
      }

      void value_separator()
      {
         assert( ! m_mode_stack.empty() );

         switch ( m_mode_stack.back() ) {
            case json_mode::ARRAY:
               return array_separator();
            case json_mode::OBJECT:
               return object_separator();
         }
         assert( false );
      }

      const std::shared_ptr< json_base > & result() const
      {
         assert( m_name_stack.empty() );
         assert( m_array_stack.empty() );
         assert( m_object_stack.empty() );
         return m_result;
      }

   private:
      bool m_next_string_is_name;

      std::shared_ptr< json_base > m_result;

      std::vector< json_mode > m_mode_stack;
      std::vector< std::string > m_name_stack;

      std::vector< std::shared_ptr< array_json > > m_array_stack;
      std::vector< std::shared_ptr< object_json > > m_object_stack;
   };

   struct grammar : pegtl::must< pegtl::json::text, pegtl::eof > {};

} // examples

int main( int argc, char ** argv )
{
   for ( int i = 1; i < argc; ++i ) {
      examples::json_state state;
      pegtl::read_parser( argv[ i ] ).parse< examples::grammar, examples::json_action, examples::errors >( state );
      assert( state.result() );
      std::cout << state.result() << std::endl;
   }
   return 0;
}
