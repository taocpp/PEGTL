// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_READ_PARSER_HH
#define PEGTL_READ_PARSER_HH

#include "eol_mode.hh"
#include "string_parser.hh"
#include "internal/file_reader.hh"

namespace pegtl
{
   template< eol_mode EOL >
   class basic_read_parser
         : public basic_string_parser< EOL >
   {
   public:
      explicit
      basic_read_parser( const std::string & filename )
            : basic_string_parser< EOL >( internal::file_reader( filename ).read(), filename )
      { }

      static constexpr eol_mode eol = EOL;
   };

   using read_parser = basic_read_parser< eol_mode::LF_WITH_CRLF >;

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_read( const std::string & filename, States && ... st )
   {
      return read_parser( filename ).parse< Rule, Action, Control >( st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_read_nested( Outer & oi, const std::string & filename, States && ... st )
   {
      return basic_read_parser< Outer::eol >( filename ).template parse_nested< Rule, Action, Control >( oi, st ... );
   }

} // namespace pegtl

#endif
