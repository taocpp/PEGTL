// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PARSE_HH
#define PEGTL_PARSE_HH

#include <string>
#include <cstring>

#include "input.hh"

#include "normal.hh"
#include "nothing.hh"

#include "apply_mode.hh"

#include "internal/skip_control.hh"
#include "internal/must.hh"

namespace pegtl
{
   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse( input & in, States && ... st )
   {
      Control< internal::skip_control< internal::must< Rule > > >::template match< apply_mode::ACTION, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse( const int argc, char ** argv, States && ... st )
   {
      input in( argc, 0, argv[ argc ], argv[ argc ] + ::strlen( argv[ argc ] ), "argv" );
      parse< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse( const char * data, const char * dend, const char * source, States && ... st )
   {
      input in( 1, 0, data, dend, source );
      parse< Rule, Action, Control >( in, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse( const char * data, const std::size_t size, const char * source, States && ... st )
   {
      parse< Rule, Action, Control >( data, data + size, source, st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse( const std::string & data, const std::string & source, States && ... st )
   {
      parse< Rule, Action, Control >( data.data(), data.data() + data.size(), source.c_str(), st ... );
   }

   template< typename Input, typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse_nested( const Input & nest, const char * data, const char * dend, const char * source, States && ... st )
   {
      input in( 1, 0, data, dend, source, & nest );
      parse< Rule, Action, Control >( in, st ... );
   }

   template< typename Input, typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse_nested( const Input & nest, const char * data, const std::size_t size, const char * source, States && ... st )
   {
      parse< Rule, Action, Control >( nest, data, data + size, source, st ... );
   }

   template< typename Input, typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   void parse_nested( const Input & nest, const std::string & data, const std::string & source, States && ... st )
   {
      parse< Rule, Action, Control >( nest, data.data(), data.data() + data.size(), source.c_str(), st ... );
   }

} // pegtl

#endif
