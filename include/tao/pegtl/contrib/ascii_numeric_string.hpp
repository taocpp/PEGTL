// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ASCII_NUMERIC_HPP
#define TAO_PEGTL_CONTRIB_ASCII_NUMERIC_HPP

#include <cstddef>

#include "../analysis/generic.hpp"
#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      constexpr std::size_t digits( const std::uint64_t N ) noexcept
      {
         return ( N < 10 ) ? 1 : ( 1 + digits( N / 10 ) );
      }

   }  // namespace internal

   inline namespace ascii
   {
      template< std::uint64_t Maximum >
      struct numeric_string
      {
         using analyze_t = analysis::generic< analysis::rule_type::any >;

         template< typename Input >
         [[nodiscard]] static bool match( Input& in ) noexcept( noexcept( in.size( 0 ) ) )
         {
            if( in.empty() ) {
               return false;
            }
            const auto c = in.peek_char( 0 );
            if( !std::isdigit( c ) ) {
               return false;
            }
            if( c == '0' ) {
               in.bump_in_this_line( 1 );
               return true;
            }
            const auto s = in.size( internal::digits( Maximum ) );
            std::size_t p = 1;
            while( ( p < s ) && std::isdigit( in.peek_char( p ) ) ) {
               ++p;
            }
            if( p == s ) {
               const std::string v( in.begin(), p );
               errno = 0;
               const auto r = std::strtoull( v.c_str(), nullptr, 10 );
               if( ( errno == ERANGE ) || !( r <= Maximum ) ) {
                  return false;
               }
            }
            in.bump_in_this_line( p );
            return true;
         }
      };

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#endif
