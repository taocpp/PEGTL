// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_DEMANGLE_CLANG_HPP
#define TAO_PEGTL_INTERNAL_DEMANGLE_CLANG_HPP

#include <cstddef>
#include <string_view>

#include "../config.hpp"

#if defined( _LIBCPP_VERSION ) || ( __clang_major__ >= 13 )

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = sv.find( '=' );
      static_assert( begin != std::string_view::npos );
      return sv.substr( begin + 2, sv.size() - begin - 3 );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#else

namespace TAO_PEGTL_NAMESPACE
{
   // When using libstdc++ with clang, std::string_view::find is not constexpr :(

   namespace special
   {
      template< char C >
      [[nodiscard]] constexpr const char* string_view_find( const char* p, std::size_t n ) noexcept
      {
         while( n ) {
            if( *p == C ) {
               return p;
            }
            ++p;
            --n;
         }
         return nullptr;
      }

   }  // namespace special

   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = special::string_view_find< '=' >( sv.data(), sv.size() );
      static_assert( begin != nullptr );
      return { begin + 2, sv.data() + sv.size() - begin - 3 };
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif

#endif
