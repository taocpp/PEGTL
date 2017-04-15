// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_STRING_PARSER_HPP
#define TAOCPP_PEGTL_INCLUDE_STRING_PARSER_HPP

#include <string>
#include <utility>

#include "config.hpp"
#include "eol.hpp"
#include "memory_input.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"
#include "position_tracking.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol = lf_crlf_eol, position_tracking P = position_tracking::IMMEDIATE >
      class string_parser
      {
      public:
         string_parser( std::string data, std::string in_source, const std::size_t byte = 0, const std::size_t line = 1, const std::size_t byte_in_line = 0 ) noexcept
            : m_data( std::move( data ) ),
              m_source( std::move( in_source ) ),
              m_input( { byte, line, byte_in_line, m_data.data() }, m_data.data() + m_data.size(), m_source.c_str() )
         {
         }

         const std::string& source() const noexcept
         {
            return m_source;
         }

         const memory_input< Eol, P >& input() const noexcept
         {
            return m_input;
         }

         template< typename Rule,
                   template< typename... > class Action = nothing,
                   template< typename... > class Control = normal,
                   apply_mode A = apply_mode::ACTION,
                   rewind_mode M = rewind_mode::REQUIRED,
                   typename... States >
         bool parse( States&&... st )
         {
            return TAOCPP_PEGTL_NAMESPACE::parse< Rule, Action, Control, A, M >( m_input, st... );
         }

         template< typename Rule,
                   template< typename... > class Action = nothing,
                   template< typename... > class Control = normal,
                   apply_mode A = apply_mode::ACTION,
                   rewind_mode M = rewind_mode::REQUIRED,
                   typename Outer,
                   typename... States >
         bool parse_nested( const Outer& oi, States&&... st )
         {
            return TAOCPP_PEGTL_NAMESPACE::parse_nested< Rule, Action, Control, A, M >( oi, m_input, st... );
         }

      private:
         const std::string m_data;
         const std::string m_source;
         memory_input< Eol, P > m_input;
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
