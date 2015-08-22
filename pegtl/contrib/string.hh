// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

// Inspired by https://github.com/irrequietus/typestring
// Rewritten and reduced to what is needed for the PEGTL
// and to work with Visual Studio 2015.

#ifndef PEGTL_CONTRIB_STRING_HH
#define PEGTL_CONTRIB_STRING_HH

#include "../ascii.hh"

namespace pegtl
{
   namespace internal
   {
      template< std::size_t N, std::size_t M >
      constexpr char ct_at( const char(&c)[ M ] ) noexcept
      {
         static_assert( M < 100, "string too long" );
         return ( N < M ) ? c[ N ] : 0;
      }

      template< typename, char ... >
      struct ct_concat;

      template< typename T >
      struct ct_concat< T >
      {
         using type = T;
      };

      template< char ... Hs, char C, char ... Cs >
      struct ct_concat< pegtl::string< Hs ... >, C, Cs ... >
            : std::conditional< C == '\0',
                                ct_concat< pegtl::string< Hs ... > >,
                                ct_concat< pegtl::string< Hs ..., C >, Cs ... > >::type
      { };

      template< char ... Cs >
      pegtl::istring< Cs ... > ct_to_istring( pegtl::string< Cs ... > );

   } // internal

} // pegtl

#define PEGTL_INTERNAL_STRING(n,x)                     \
   pegtl::internal::ct_at< n##0 >( x ),                \
   pegtl::internal::ct_at< n##1 >( x ),                \
   pegtl::internal::ct_at< n##2 >( x ),                \
   pegtl::internal::ct_at< n##3 >( x ),                \
   pegtl::internal::ct_at< n##4 >( x ),                \
   pegtl::internal::ct_at< n##5 >( x ),                \
   pegtl::internal::ct_at< n##6 >( x ),                \
   pegtl::internal::ct_at< n##7 >( x ),                \
   pegtl::internal::ct_at< n##8 >( x ),                \
   pegtl::internal::ct_at< n##9 >( x )

#define PEGTL_STRING(x)                                                 \
   pegtl::internal::ct_concat< pegtl::string<>,                         \
                               PEGTL_INTERNAL_STRING(,x),               \
                               PEGTL_INTERNAL_STRING(1,x),              \
                               PEGTL_INTERNAL_STRING(2,x),              \
                               PEGTL_INTERNAL_STRING(3,x),              \
                               PEGTL_INTERNAL_STRING(4,x),              \
                               PEGTL_INTERNAL_STRING(5,x),              \
                               PEGTL_INTERNAL_STRING(6,x),              \
                               PEGTL_INTERNAL_STRING(7,x),              \
                               PEGTL_INTERNAL_STRING(8,x),              \
                               PEGTL_INTERNAL_STRING(9,x) >::type

#define PEGTL_ISTRING(x)                                                \
   decltype( pegtl::internal::ct_to_istring( PEGTL_STRING(x)() ) )

#endif
