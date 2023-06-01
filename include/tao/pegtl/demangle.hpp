// Copyright (c) 2014-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEMANGLE_HPP
#define TAO_PEGTL_DEMANGLE_HPP

#include <string_view>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // ensure a consistent interface
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept;

}  // namespace TAO_PEGTL_NAMESPACE

#if defined( __clang__ )

#if defined( _LIBCPP_VERSION )

template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   constexpr std::string_view sv = __PRETTY_FUNCTION__;
   constexpr auto begin = sv.find( '=' );
   static_assert( begin != std::string_view::npos );
   return sv.substr( begin + 2, sv.size() - begin - 3 );
}

#else

namespace TAO_PEGTL_NAMESPACE::internal
{
   // When using libstdc++ with clang, std::string_view::find is not constexpr :(
   template< char C >
   constexpr const char* string_view_find( const char* p, std::size_t n ) noexcept
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

}  // namespace TAO_PEGTL_NAMESPACE::internal

template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   constexpr std::string_view sv = __PRETTY_FUNCTION__;
   constexpr auto begin = internal::string_view_find< '=' >( sv.data(), sv.size() );
   static_assert( begin != nullptr );
   return { begin + 2, sv.data() + sv.size() - begin - 3 };
}

#endif

#elif defined( __GNUC__ )

#if( __GNUC__ == 9 ) && ( __GNUC_MINOR__ < 3 )

#if !defined( __cpp_rtti )
#error "RTTI support required for GCC 9.1/9.2"
#else

#include <typeinfo>

// GCC 9.1 and 9.2 have a bug that leads to truncated __PRETTY_FUNCTION__ names,
// see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91155
template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   // fallback: requires RTTI, no demangling
   return typeid( T ).name();
}

#endif

#else

namespace TAO_PEGTL_NAMESPACE::special
{
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept
   {
      constexpr std::string_view sv = __PRETTY_FUNCTION__;
      constexpr auto begin = sv.find( '=' );
      static_assert( begin != std::string_view::npos );
      constexpr auto tmp = sv.substr( begin + 2 );
      constexpr auto end = tmp.rfind( ';' );
      static_assert( end != std::string_view::npos );
      return tmp.substr( 0, end );
   }

}  // namespace TAO_PEGTL_NAMESPACE::special

template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   return TAO_PEGTL_NAMESPACE::special::demangle< T >();
}

#endif

#elif defined( _MSC_VER )

template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   // we can not add static_assert for additional safety,
   // see issues #296, #301 and #308
   constexpr std::string_view sv = __FUNCSIG__;
   constexpr auto begin = sv.find( "demangle<" );
   constexpr auto tmp = sv.substr( begin + 9 );
   constexpr auto end = tmp.rfind( '>' );
   return tmp.substr( 0, end );
}

#else

#if !defined( __cpp_rtti )
#error "RTTI support required for unknown compilers"
#else

#include <typeinfo>

template< typename T >
[[nodiscard]] constexpr std::string_view TAO_PEGTL_NAMESPACE::demangle() noexcept
{
   // fallback: requires RTTI, no demangling
   return typeid( T ).name();
}

#endif

#endif

#endif
