// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RESIZE_UNINITIALIZED_HPP
#define TAO_PEGTL_INTERNAL_RESIZE_UNINITIALIZED_HPP

#include <cstddef>
#include <string>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   // the below uses a hack to call private member functions of a class, described here:
   // https://github.com/facebook/folly/blob/master/folly/memory/UninitializedMemoryHacks.h

   namespace  // NOLINT(google-build-namespaces)
   {
      // declare some functions...
      void resize_uninitialized_proxy( std::string& v, const std::size_t n );
      void resize_uninitialized_proxy( std::basic_string< unsigned char >& v, const std::size_t n );
      void resize_uninitialized_proxy( std::basic_string< std::byte >& v, const std::size_t n );

#if defined( _LIBCPP_STRING )

      // ...create a proxy to generate the actual implementation of the above function...
      template< typename T, void ( T::*F )( std::size_t ) >
      struct proxy
      {
         // ...define the function declared above...
         friend void resize_uninitialized_proxy( T& v, const std::size_t n )
         {
            ( v.*F )( n );  // v.__set_size( n );
            v[ v.size() ] = typename T::value_type( 0 );
         }
      };

      // ...and here's the actual "trick": an explicit template instantiation skips the access checks,
      // so you can reference private members and forward them to the above proxy!
      template struct proxy< std::string, &std::string::__set_size >;
      template struct proxy< std::basic_string< unsigned char >, &std::basic_string< unsigned char >::__set_size >;
      template struct proxy< std::basic_string< std::byte >, &std::basic_string< std::byte >::__set_size >;

#elif defined( _GLIBCXX_STRING ) && _GLIBCXX_USE_CXX11_ABI

      template< typename T, void ( T::*F )( std::size_t ) >
      struct proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n )
         {
            ( v.*F )( n );  // v._M_set_length( n );
         }
      };

      template struct proxy< std::string, &std::string::_M_set_length >;
      template struct proxy< std::basic_string< unsigned char >, &std::basic_string< unsigned char >::_M_set_length >;
      template struct proxy< std::basic_string< std::byte >, &std::basic_string< std::byte >::_M_set_length >;

#elif defined( _GLIBCXX_STRING )

      template< typename T,
                typename R,
                R* ( T::*F )() const >
      struct proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n )
         {
            // v._M_rep()->_M_set_length_and_sharable( n );
            ( v.*F )()->_M_set_length_and_sharable( n );
         }
      };

      template struct proxy< std::string,
                             std::string::_Rep,
                             &std::string::_M_rep >;

      template struct proxy< std::basic_string< unsigned char >,
                             std::basic_string< unsigned char >::_Rep,
                             &std::basic_string< unsigned char >::_M_rep >;

      template struct proxy< std::basic_string< std::byte >,
                             std::basic_string< std::byte >::_Rep,
                             &std::basic_string< std::byte >::_M_rep >;

#elif defined( _MSC_VER )

      template< typename T, void ( T::*F )( std::size_t ) >
      struct proxy
      {
         friend void resize_uninitialized_proxy( T& v, const std::size_t n )
         {
            ( v.*F )( n );  // v._Eos( n );
         }
      };

      template struct proxy< std::string, &std::string::_Eos >;
      template struct proxy< std::basic_string< unsigned char >, &std::basic_string< unsigned char >::_Eos >;
      template struct proxy< std::basic_string< std::byte >, &std::basic_string< std::byte >::_Eos >;

#else
#error "No implementation for resize_uninitialized available."
#endif

   }  // namespace

   template< typename T >
   void resize_uninitialized( std::basic_string< T >& v, const std::size_t n )
   {
      if( n <= v.size() ) {
         v.resize( n );
      }
      else {
         // careful not to call reserve() unless necessary, as it causes shrink_to_fit() on many platforms
         if( n > v.capacity() ) {
            v.reserve( n );
         }
         internal::resize_uninitialized_proxy( v, n );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
