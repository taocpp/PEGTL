// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP
#define TAO_PEGTL_CONTRIB_INTERNAL_PRINT_UTILITY_HPP

#include <cassert>
#include <cstdio>
#include <ostream>
#include <string_view>

#include "../../config.hpp"
#include "../../demangle.hpp"

#include "../forward.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      inline constexpr const std::string_view reset_style = "\033[m";

      struct styled
      {
         const std::string_view style;
         const std::string_view string;
      };

      inline std::ostream& operator<<( std::ostream& os, const styled ss )
      {
         os << ss.style << ss.string << reset_style;
         return os;
      }

      struct print_rules_config
      {
         static constexpr int width = 21;

         static constexpr std::string_view pegtl_style = "\033[38;5;238m";  // "\033[90m";
         static constexpr std::string_view quote_style = "\033[1m";
         static constexpr std::string_view user_style = "\033[38;5;18m";  // "\033[34m";

         print_rules_config() = default;

         explicit print_rules_config( const std::string_view in_include )
            : include( in_include )
         {}

         static styled pegtl( const std::string_view string ) noexcept
         {
            return { pegtl_style, string };
         }

         static styled user( const std::string_view string ) noexcept
         {
            return { user_style, string };
         }

         const std::string_view include;
         const std::string_view exclude = "tao::pegtl::";

         template< typename Rule >
         [[nodiscard]] std::string_view name() const noexcept
         {
            const std::string_view d = demangle< Rule >();
            if( ( !is_in_namespace( d, include ) ) && is_in_namespace( d, exclude ) ) {
               return std::string_view();
            }
            const std::string_view::size_type b = d.find( '<' );
            const std::string_view::size_type c = d.rfind( ':', b );
            return ( c == std::string_view::npos ) ? d : d.substr( c + 1 );
         }

      private:
         [[nodiscard]] static bool is_in_namespace( const std::string_view rule_name, const std::string_view name_space ) noexcept
         {
            // TODO: Check whether this needs tweaking for Windows and/or MSVC.
            return ( rule_name.size() > name_space.size() ) && ( rule_name.compare( 0, name_space.size(), name_space ) == 0 );
         }
      };

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
            const auto s = std::snprintf( b, sizeof( b ), "\\u%04x", unsigned( i ) );
            os.write( b, s );
            return;
         }
         if( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "\\U%06x", unsigned( i ) );
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
         if( i < 0x110000 ) {
            char b[ 10 ];
            const auto s = std::snprintf( b, sizeof( b ), "U+%X", unsigned( i ) );
            os.write( b, s );
            return;
         }
         assert( false );  // Or what?
      }

      template< typename Rule >
      void print_rules_rule( std::ostream& os, const print_rules_config& pc )
      {
         if( const auto rule = pc.template name< Rule >(); !rule.empty() ) {
            os << pc.user( rule );
         }
         else {
            print_rules_traits< typename Rule::rule_t >::print( os, pc );
         }
      }

      template< typename Rule, typename... Rules >
      void print_rules_rules( std::ostream& os, const print_rules_config& pc, const char* a, const char* b = "( ", [[maybe_unused]] const char* c = ", ", const char* d = " )" )
      {
         if( a ) {
            os << pc.pegtl( a );
         }
         os << b;
         ( print_rules_rule< Rule >( os, pc ), ..., ( os << c, print_rules_rule< Rules >( os, pc ) ) );
         os << d;
      }

   }  // namespace internal

}  // namespace TAO_PEGTL_NAMESPACE

#endif
