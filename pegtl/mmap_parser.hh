// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_MMAP_PARSER_HH
#define PEGTL_MMAP_PARSER_HH

#include <string>

#include "parse.hh"
#include "normal.hh"
#include "nothing.hh"
#include "eol.hh"
#include "internal/file_mapper.hh"

namespace pegtl
{
   template< typename Eol >
   class basic_mmap_parser
   {
   public:
      explicit
      basic_mmap_parser( const std::string & filename )
            : m_file( filename ),
              m_source( filename ),
              m_input( 0, 1, 0, m_file.begin(), m_file.end(), m_source.c_str() )
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
      internal::file_mapper m_file;
      std::string m_source;
      basic_memory_input< Eol > m_input;
   };

   using mmap_parser = basic_mmap_parser< lf_crlf_eol >;

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
   bool parse_mmap( const std::string & filename, States && ... st )
   {
      return mmap_parser( filename ).parse< Rule, Action, Control >( st ... );
   }

   template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename Outer, typename ... States >
   bool parse_mmap_nested( Outer & oi, const std::string & filename, States && ... st )
   {
      return basic_mmap_parser< typename Outer::eol >( filename ).template parse_nested< Rule, Action, Control >( oi, st ... );
   }

} // namespace pegtl

#endif
