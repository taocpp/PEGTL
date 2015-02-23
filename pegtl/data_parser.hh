// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_DATA_PARSER_HH
#define PEGTL_DATA_PARSER_HH

#include "internal/skip_control.hh"
#include "internal/must.hh"

namespace pegtl
{
   class data_parser
   {
   public:
      data_parser( std::string && data, const std::string & source )
            : m_data( std::move( data ) ),
              m_source( source ),
              m_input( 1, 0, m_data.data(), m_data.data() + m_data.size(), m_source.c_str() )
      { }

      data_parser( const std::string & data, const std::string & source )
            : m_data( data ),
              m_source( source ),
              m_input( 1, 0, m_data.data(), m_data.data() + m_data.size(), m_source.c_str() )
      { }

      data_parser( std::string && data, const std::string & source, const input & from )
            : m_data( std::move( data ) ),
              m_source( source ),
              m_input( 1, 0, m_data.data(), m_data.data() + m_data.size(), m_source.c_str(), & from )
      { }

      data_parser( const std::string & data, const std::string & source, const input & from )
            : m_data( data ),
              m_source( source ),
              m_input( 1, 0, m_data.data(), m_data.data() + m_data.size(), m_source.c_str(), & from )
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
         Control< internal::skip_control< internal::must< Rule > > >::template match< Action, Control >( m_input, st ... );
      }

   private:
      std::string m_data;
      std::string m_source;
      pegtl::input m_input;
   };

} // pegtl

#endif
