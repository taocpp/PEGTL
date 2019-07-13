// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_DEMANGLE_HPP
#define TAO_PEGTL_INTERNAL_DEMANGLE_HPP

#include <ciso646>
#include <string_view>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< char C >
   constexpr const char* find( const char* p, std::size_t n ) noexcept
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

   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
#if defined( __clang__ )

#if defined( _LIBCPP_VERSION )

      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = sv.find( '=' );
      static_assert( begin != std::string_view::npos );
      return sv.substr( begin + 2, sv.size() - begin - 3 );

#else

      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      // When using libstdc++ with clang, std::string_view::find is not constexpr :(
      constexpr auto begin = find< '=' >( sv.data(), sv.size() );
      static_assert( begin != nullptr );
      return { begin + 2, sv.end() - begin - 3 };

#endif

#elif defined( __GNUC__ ) && !( ( __GNUC__ == 9 ) && ( __GNUC_MINOR__ == 1 ) )
      // GCC 9.1 has a bug that leads to truncated __PRETTY_FUNCTION__ names,
      // see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91155

      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = sv.find( '=' );
      static_assert( begin != std::string_view::npos );
      constexpr auto tmp = sv.substr( begin + 2 );
      constexpr auto end = tmp.rfind( ';' );
      static_assert( end != std::string_view::npos );
      return tmp.substr( 0, end );

#elif defined( _MSC_VER )

      constexpr std::string_view sv = __FUNCSIG__;
      constexpr auto begin = sv.find( "demangle<" );
      static_assert( begin != std::string_view::npos );
      constexpr auto tmp = sv.substr( begin + 9 );
      constexpr auto end = tmp.rfind( '>' );
      static_assert( end != std::string_view::npos );
      return tmp.substr( 0, end );

#else

      // fallback: requires RTTI, no demangling
      return typeid( T ).name();

#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
