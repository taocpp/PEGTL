// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_STRING_PARSER_HH
#define PEGTL_STRING_PARSER_HH

#include <string>
#include <utility>

#include "parse.hh"
#include "normal.hh"
#include "nothing.hh"
#include "eol.hh"
#include "memory_input.hh"

namespace pegtl
{
   template< typename Eol >
   class basic_string_parser
   {
   public:
      basic_string_parser( std::string data, std::string in_source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t byte_in_line = 0 )
            : m_data( std::move( data ) ),
              m_source( std::move( in_source ) ),
              m_input( byte, line, byte_in_line, m_data.data(), m_data.data() + m_data.size(), m_source.c_str() )
      { }

      const std::string & source() const
      {
         return m_source;
      }

      const memory_input & input() const
      {
         return m_input;
      }

      template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
      bool parse( States && ... st )
      {
         return parse_input< Rule, Action, Control >( m_input, st ... );
      }

      template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
      bool parse_nested( Outer & oi, States && ... st )
      {
         return parse_input_nested< Rule, Action, Control >( oi, m_input, st ... );
      }

      using eol = Eol;

   private:
      std::string m_data;
      std::string m_source;
      basic_memory_input< Eol > m_input;
   };

   using string_parser = basic_string_parser< lf_crlf_eol >;

} // namespace pegtl

#endif
