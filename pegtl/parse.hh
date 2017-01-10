// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PARSE_HH
#define PEGTL_PARSE_HH

#include <string>
#include <cstring>
#include <sstream>

#include "normal.hh"
#include "nothing.hh"
#include "apply_mode.hh"
#include "rewind_mode.hh"
#include "memory_input.hh"
#include "buffer_input.hh"

#include "internal/cstream_reader.hh"
#include "internal/cstring_reader.hh"
#include "internal/istream_reader.hh"

namespace pegtl
{
   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Input, typename ... States >
   bool parse_input( Input & in, States && ... st )
   {
      return Control< Rule >::template match< apply_mode::ACTION, rewind_mode::REQUIRED, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_arg( const int argn, char ** argv, States && ... st )
   {
      std::ostringstream os;
      os << "argv[" << argn << ']';
      const std::string source = os.str();
      assert( argv[ argn ] );
      memory_input in( 0, 1, 0, argv[ argn ], argv[ argn ] + std::strlen( argv[ argn ] ), source.c_str() );
      return parse_input< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_memory( const char * data, const char * dend, const char * source, States && ... st )
   {
      memory_input in( 0, 1, 0, data, dend, source );
      return parse_input< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_memory( const char * data, const std::size_t size, const char * source, States && ... st )
   {
      return parse_memory< Rule, Action, Control >( data, data + size, source, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_string( const std::string & data, const std::string & source, States && ... st )
   {
      return parse_memory< Rule, Action, Control >( data.data(), data.data() + data.size(), source.c_str(), st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_cstream( std::FILE * stream, const char * source, const std::size_t maximum, States && ... st )
   {
      buffer_input< internal::cstream_reader > in( source, maximum, stream );
      return parse_input< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_stdin( const std::size_t maximum, States && ... st )
   {
      return parse_cstream< Rule, Action, Control >( stdin, "stdin", maximum, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_cstring( const char * string, const char * source, const std::size_t maximum, States && ... st )
   {
      buffer_input< internal::cstring_reader > in( source, maximum, string );
      return parse_input< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_istream( std::istream & stream, const std::string & source, const std::size_t maximum, States && ... st )
   {
      buffer_input< internal::istream_reader > in( source.c_str(), maximum, stream );
      return parse_input< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename Input, typename ... States >
   bool parse_input_nested( const Outer & oi, Input & in, States && ... st )
   {
      try {
         return Control< Rule >::template match< apply_mode::ACTION, rewind_mode::REQUIRED, Action, Control >( in, st ... );
      }
      catch ( parse_error & e ) {
         e.positions.push_back( oi.position() );
         throw;
      }
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_memory_nested( const Outer & oi, const char * data, const char * dend, const char * source, States && ... st )
   {
      basic_memory_input< typename Outer::eol_t > in( 0, 1, 0, data, dend, source );
      return parse_input_nested< Rule, Action, Control >( oi, in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_memory_nested( const Outer & oi, const char * data, const std::size_t size, const char * source, States && ... st )
   {
      return parse_memory_nested< Rule, Action, Control >( oi, data, data + size, source, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_string_nested( const Outer & oi, const std::string & data, const std::string & source, States && ... st )
   {
      return parse_memory_nested< Rule, Action, Control >( oi, data.data(), data.data() + data.size(), source.c_str(), st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_cstream_nested( const Outer & oi, std::FILE * stream, const char * source, const std::size_t maximum, States && ... st )
   {
      basic_buffer_input< typename Outer::eol_t, internal::cstream_reader > in( source, maximum, stream );
      return parse_input_nested< Rule, Action, Control >( oi, in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_cstring_nested( const Outer & oi, const char * string, const char * source, const std::size_t maximum, States && ... st )
   {
      basic_buffer_input< typename Outer::eol_t, internal::cstring_reader > in( source, maximum, string );
      return parse_input_nested< Rule, Action, Control >( oi, in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_istream( const Outer & oi, std::istream & stream, const std::string & source, const std::size_t maximum, States && ... st )
   {
      basic_buffer_input< typename Outer::eol_t, internal::istream_reader > in( source.c_str(), maximum, stream );
      return parse_input_nested< Rule, Action, Control >( oi, in, st ... );
   }

} // namespace pegtl

#endif
