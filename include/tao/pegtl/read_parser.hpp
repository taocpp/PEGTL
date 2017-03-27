// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_READ_PARSER_HPP
#define TAOCPP_PEGTL_INCLUDE_READ_PARSER_HPP

#include "config.hpp"
#include "eol.hpp"
#include "string_parser.hpp"

#include "internal/file_reader.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      template< typename Eol >
      class basic_read_parser
         : public basic_string_parser< Eol >
      {
      public:
         explicit basic_read_parser( const std::string& filename )
            : basic_string_parser< Eol >( internal::file_reader( filename ).read(), filename )
         {
         }

         using eol = Eol;
      };

      using read_parser = basic_read_parser< lf_crlf_eol >;

      template< typename Rule, template< typename... > class Action = nothing, template< typename... > class Control = normal, typename... States >
      bool parse_read( const std::string& filename, States&&... st )
      {
         return read_parser( filename ).parse< Rule, Action, Control >( st... );
      }

      template< typename Rule, template< typename... > class Action = nothing, template< typename... > class Control = normal, typename Outer, typename... States >
      bool parse_read_nested( Outer& oi, const std::string& filename, States&&... st )
      {
         return basic_read_parser< typename Outer::eol >( filename ).template parse_nested< Rule, Action, Control >( oi, st... );
      }

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
