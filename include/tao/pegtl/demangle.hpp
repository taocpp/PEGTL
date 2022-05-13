// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEMANGLE_HPP
#define TAO_PEGTL_DEMANGLE_HPP

#include <ciso646>
#include <string_view>

namespace tao::pegtl
{
   // ensure a consistent interface
   template< typename T >
   [[nodiscard]] constexpr std::string_view demangle() noexcept;

}  // namespace tao::pegtl

#if defined( __clang__ )

#if defined( _LIBCPP_VERSION )

template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   constexpr std::string_view sv = __PRETTY_FUNCTION__;
   constexpr auto begin = sv.find( '=' );
   static_assert( begin != std::string_view::npos );
   return sv.substr( begin + 2, sv.size() - begin - 3 );
}

#else

namespace tao::pegtl::internal
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

}  // namespace tao::pegtl::internal

template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
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
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   // fallback: requires RTTI, no demangling
   return typeid( T ).name();
}

#endif

#else

template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   constexpr std::string_view sv = __PRETTY_FUNCTION__;
   constexpr auto begin = sv.find( '=' );
   static_assert( begin != std::string_view::npos );
   constexpr auto tmp = sv.substr( begin + 2 );
   constexpr auto end = tmp.rfind( ';' );
   static_assert( end != std::string_view::npos );
   return tmp.substr( 0, end );
}

#endif

#elif defined( _MSC_VER )

#if( _MSC_VER < 1932 )

template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   constexpr std::string_view sv = __FUNCSIG__;
   constexpr auto begin = sv.find( "demangle<" );
   // see issues #296, #301 and #308
   if constexpr( begin != std::string_view::npos ) {
      constexpr auto tmp = sv.substr( begin + 9 );
      constexpr auto end = tmp.rfind( '>' );
      static_assert( end != std::string_view::npos );
      return tmp.substr( 0, end );
   }
}

#else

#if !defined( __cpp_rtti )
#error "RTTI support required for MSVC 19.32"
#else

#include <typeinfo>

// MSVC 19.32 has a bug where __FUNCSIG__ does not include the function's template parameters,
// see https://developercommunity.visualstudio.com/t/C-__FUNCSIG__-macro-does-not-include-t/10040930
template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   // fallback: requires RTTI, no demangling
   return typeid( T ).name();
}

#endif

#endif

#else

#if !defined( __cpp_rtti )
#error "RTTI support required for unknown compilers"
#else

#include <typeinfo>

template< typename T >
[[nodiscard]] constexpr std::string_view tao::pegtl::demangle() noexcept
{
   // fallback: requires RTTI, no demangling
   return typeid( T ).name();
}

#endif

#endif

#endif
