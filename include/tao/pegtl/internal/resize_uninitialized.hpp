// Copyright (c) 2021-2025 Daniel Frey and Dr. Colin Hirsch
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RESIZE_UNINITIALIZED_HPP
#define TAO_PEGTL_INTERNAL_RESIZE_UNINITIALIZED_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace TAO_PEGTL_NAMESPACE::internal
{
   // This uses a hack to call private member functions of a class, described here:
   // https://github.com/facebook/folly/blob/master/folly/memory/UninitializedMemoryHacks.h

   namespace  // NOLINT(google-build-namespaces)
   {
      struct odr_helper;

      void resize_uninitialized_proxy( std::string& v, const std::size_t n ) noexcept;

#if defined( _LIBCPP_STRING )

      // Create a proxy to generate the actual implementation of the above function...

      template< typename T, auto F >
      struct string_proxy
      {
         // ...define the function declared above...
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            ( v.*F )( n );  // v.__set_size( n );
            v[ v.size() ] = typename T::value_type( 0 );
         }
      };

      // ...the actual "trick": an explicit template instantiation skips the access checks,
      // so you can reference private members and forward them to the above proxy!

      template struct string_proxy< std::string, &std::string::__set_size >;

#elif defined( _GLIBCXX_STRING ) && _GLIBCXX_USE_CXX11_ABI  // NOLINT(misc-include-cleaner)

      template< typename T, auto F >
      struct string_proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            ( v.*F )( n );  // v._M_set_length( n );
         }
      };

      template struct string_proxy< std::string, &std::string::_M_set_length >;

#elif defined( _GLIBCXX_STRING )

      template< typename T, auto F >
      struct string_proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            // v._M_rep()->_M_set_length_and_sharable( n );
            ( v.*F )()->_M_set_length_and_sharable( n );
         }
      };

      template struct string_proxy< std::string, &std::string::_M_rep >;

#elif defined( _MSC_VER )

      template< typename T, auto F >
      struct string_proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            ( v.*F )( n );  // v._Eos( n );
         }
      };

      template struct string_proxy< std::string, &std::string::_Eos >;

#else
#error "No implementation for resize_uninitialized for std::string available on this platform."
#endif

#if defined( _LIBCPP_VECTOR )

      void resize_uninitialized_proxy( std::vector< std::byte >& v, const std::size_t n ) noexcept;

      template< typename T, auto M >
      struct vector_proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            // v.__end_ = v.data() + n;
            v.*M = v.data() + n;

#ifndef _LIBCPP_HAS_NO_ASAN
            __sanitizer_annotate_contiguous_container( v.data(),
                                                       v.data() + v.capacity(),
                                                       v.data() + v.size(),
                                                       v.data() + n );
#endif
         }
      };

      template struct vector_proxy< std::vector< std::byte >, &std::vector< std::byte >::__end_ >;

#elif defined( _MSC_VER )

      void resize_uninitialized_proxy( std::vector< std::byte >& v, const std::size_t n ) noexcept;

      template< typename T, auto Mypair, auto Myval2, auto Mylast >
      struct vector_proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n ) noexcept
         {
            // v._Mypair._Myval2._Mylast = v.data() + n;
            v.*Mypair.*Myval2.*Mylast = v.data() + n;
         }
      };

      template struct vector_proxy< std::vector< std::byte >,
                                    &std::vector< std::byte >::_Mypair,
                                    &decltype( std::declval< std::vector< std::byte > >()._Mypair )::_Myval2,
                                    &decltype( std::declval< std::vector< std::byte > >()._Mypair._Myval2 )::_Mylast >;

#else

      // generic version

      struct no_init_byte
      {
         std::byte b;
         no_init_byte() noexcept {}  // NOLINT(modernize-use-equals-default)
      };

      static_assert( sizeof( std::vector< std::byte > ) == sizeof( std::vector< no_init_byte > ) );
      static_assert( alignof( std::vector< std::byte > ) == alignof( std::vector< no_init_byte > ) );

      inline void resize_uninitialized_proxy( std::vector< std::byte >& v, const std::size_t n ) noexcept
      {
         // undefined behaviour?
         reinterpret_cast< std::vector< no_init_byte >& >( v ).resize( n );
      }

#endif

   }  // namespace

   template< typename = odr_helper >
   void resize_uninitialized( std::string& v, const std::size_t n )
   {
      if( n <= v.size() ) {
         v.resize( n );
      }
      else {
         if( n > v.capacity() ) {
            v.reserve( n );
         }
         internal::resize_uninitialized_proxy( v, n );
      }
   }

   template< typename = odr_helper >
   void resize_uninitialized( std::vector< std::byte >& v, const std::size_t n )
   {
      if( n <= v.size() ) {
         v.resize( n );
      }
      else {
         if( n > v.capacity() ) {
            v.reserve( n );
         }
         internal::resize_uninitialized_proxy( v, n );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
