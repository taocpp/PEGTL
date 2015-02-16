// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_READ_PARSER_HH
#define PEGTL_READ_PARSER_HH

#include "apply_mode.hh"
#include "error_mode.hh"

#include "internal/file_reader.hh"

namespace pegtl
{
   class read_parser
   {
   public:
      explicit
      read_parser( const std::string & filename )
            : m_file( internal::file_reader( filename ).read() ),
              m_source( filename ),
              m_input( 1, 0, m_file.data(), m_file.data() + m_file.size(), m_source.c_str() )
      { }

      read_parser( const std::string & filename, const input & from )
            : m_file( internal::file_reader( filename ).read() ),
              m_source( filename ),
              m_input( 1, 0, m_file.data(), m_file.data() + m_file.size(), m_source.c_str(), & from )
      { }

      const std::string & source() const
      {
         return m_source;
      }

      const pegtl::input & input() const
      {
         return m_input;
      }

      template< typename Rule, template< typename ... > class Action = nothing, template< typename ... > class Control = normal, typename ... States >
      void parse( States && ... st )
      {
         Control< Rule >::template match< apply_mode::ACTION, error_mode::THROW, Action, Control >( m_input, st ... );
      }

   private:
      std::string m_file;
      std::string m_source;
      pegtl::input m_input;
   };

} // pegtl

#endif
