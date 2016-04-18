// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_READ_PARSER_HH
#define PEGTL_READ_PARSER_HH

#include "string_parser.hh"

#include "internal/file_reader.hh"

namespace pegtl
{
   class read_parser
         : public string_parser
   {
   public:
      explicit
      read_parser( const std::string & filename )
            : string_parser( internal::file_reader( filename ).read(), filename )
      { }
   };

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_read( const std::string & filename, States && ... st )
   {
      return read_parser( filename ).parse< Rule, Action, Control >( st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_read_nested( Outer & oi, const std::string & filename, States && ... st )
   {
      return read_parser( filename ).parse_nested< Rule, Action, Control >( oi, st ... );
   }

} // pegtl

#endif
