// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_MMAP_PARSER_HPP
#define TAOCPP_PEGTL_INCLUDE_MMAP_PARSER_HPP

#include <string>

#include "config.hpp"
#include "eol.hpp"
#include "internal/file_mapper.hpp"
#include "normal.hpp"
#include "nothing.hpp"
#include "parse.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol >
      class basic_mmap_parser
      {
      public:
         explicit basic_mmap_parser( const std::string& filename )
            : m_file( filename ),
              m_source( filename ),
              m_input( m_file.begin(), m_file.end(), m_source.c_str() )
         {
         }

         const std::string& source() const
         {
            return m_source;
         }

         const basic_memory_input< Eol >& input() const
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

         using eol_t = Eol;

      private:
         internal::file_mapper m_file;
         std::string m_source;
         basic_memory_input< Eol > m_input;
      };

      using mmap_parser = basic_mmap_parser< lf_crlf_eol >;

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename... States >
      bool parse_mmap( const std::string& filename, States&&... st )
      {
         return mmap_parser( filename ).parse< Rule, Action, Control, A, M >( st... );
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                apply_mode A = apply_mode::ACTION,
                rewind_mode M = rewind_mode::REQUIRED,
                typename Outer,
                typename... States >
      bool parse_mmap_nested( const Outer& oi, const std::string& filename, States&&... st )
      {
         return basic_mmap_parser< typename Outer::eol_t >( filename ).template parse_nested< Rule, Action, Control, A, M >( oi, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
