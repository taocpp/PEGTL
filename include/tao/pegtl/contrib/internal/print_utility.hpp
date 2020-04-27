// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP

#include <cassert>
#include <cstdio>
#include <ostream>
#include <string_view>

#include "../../config.hpp"

#include "../forward.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      inline void print_escape( std::ostream& os, const char32_t i )
      {
         switch( i ) {
            case '"':
               os << "\\\"";
               return;
            case '\\':
               os << "\\\\";
               return;
            case '\a':
               os << "\\a";
               return;
            case '\b':
               os << "\\b";
               return;
            case '\t':
               os << "\\t";
               return;
            case '\n':
               os << "\\n";
               return;
            case '\r':
               os << "\\r";
               return;
            case '\v':
               os << "\\v";
               return;
            default:
               break;
         }
         if( ( 32 <= i ) && ( i <= 126 ) ) {
            os << char( i );
            return;
         }
         if( i < 0x10000 ) {
            char b[ 8 ];
            const auto s = std::snprintf( b, sizeof( b ), "\\u%04x", i );
            os.write( b, s );
            return;
         }
         if ( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "\\U%06x", i );
            os.write( b, s );
            return;
         }
         assert( false );  // Or what?
      }

      inline void print_escape1( std::ostream& os, const char32_t i )
      {
         switch( i ) {
            case '\'':
               os << "'\\''";
               return;
            case '\\':
               os << "'\\\\'";
               return;
            case '\a':
               os << "'\\a'";
               return;
            case '\b':
               os << "'\\b'";
               return;
            case '\t':
               os << "'\\t'";
               return;
            case '\n':
               os << "'\\n'";
               return;
            case '\r':
               os << "'\\r'";
               return;
            case '\v':
               os << "'\\v'";
               return;
            default:
               break;
         }
         if( ( 32 <= i ) && ( i <= 126 ) ) {
            os << '\'' << char( i ) << '\'';
            return;
         }
         if ( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "U+%X", i );
            os.write( b, s );
            return;
         }
         assert( false );  // Or what?
      }

      inline bool is_in_namespace( const std::string_view rule_name, const std::string_view name_space ) noexcept
      {
         // TODO: Check whether this needs tweaking for Windows and/or MSVC.
         return ( rule_name.size() > name_space.size() ) && ( rule_name.compare( 0, name_space.size(), name_space ) == 0 );
      }

      template< typename Rule >
      void print_rules_rule( std::ostream& os, const std::string_view prefix )
      {
         const std::string_view rule = demangle< Rule >();
         if( is_in_namespace( rule, prefix ) ) {
            os << rule.substr( prefix.size() );
         }
         else {
            print_rules_traits< typename Rule::rule_t >::print( os, prefix );
         }
      }

      template< typename Rule, typename... Rules >
      void print_rules_rules( std::ostream& os, const std::string_view prefix, const char* a, const char* b = "( ", const char* c = ", ", const char* d = " )", const char* e = "" )
      {
         os << a << b;
         ( print_rules_rule< Rule >( os, prefix ), ..., ( os << c, print_rules_rule< Rules >( os, prefix ) ) );
         os << d << e;
      }

   }  // namespace internal

}  // namespace TAO_PEGTL_NAMESPACE

#endif
