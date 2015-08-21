// Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

// Inspired by https://github.com/irrequietus/typestring
// Rewritten and reduced to what is needed for the PEGTL

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

      template< char... C >
      auto ct_concat( pegtl::string< C... > ) -> pegtl::string< C... >;

      template< char... H, char... T >
      auto ct_concat( pegtl::string< H... >, pegtl::string< '\0' >, pegtl::string< T >... )
        -> pegtl::string< H... >;

      template< char... H, char C, char... T >
      auto ct_concat( pegtl::string< H... >, pegtl::string< C >, pegtl::string< T >... )
        -> decltype( ct_concat( pegtl::string< H..., C >(), pegtl::string< T >()... ) );

      template< char... C >
      auto ct_to_istring( pegtl::string< C... > ) -> pegtl::istring< C... >;

   } // internal

} // pegtl

#define PEGTL_INTERNAL_STRING(n,x)                                      \
   pegtl::string< pegtl::internal::ct_at< n##0 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##1 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##2 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##3 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##4 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##5 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##6 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##7 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##8 >( x ) >(),              \
   pegtl::string< pegtl::internal::ct_at< n##9 >( x ) >()

#define PEGTL_STRING(x)                                                 \
   decltype( pegtl::internal::ct_concat( PEGTL_INTERNAL_STRING(,x),     \
                                         PEGTL_INTERNAL_STRING(1,x),    \
                                         PEGTL_INTERNAL_STRING(2,x),    \
                                         PEGTL_INTERNAL_STRING(3,x),    \
                                         PEGTL_INTERNAL_STRING(4,x),    \
                                         PEGTL_INTERNAL_STRING(5,x),    \
                                         PEGTL_INTERNAL_STRING(6,x),    \
                                         PEGTL_INTERNAL_STRING(7,x),    \
                                         PEGTL_INTERNAL_STRING(8,x),    \
                                         PEGTL_INTERNAL_STRING(9,x) ) )

#define PEGTL_ISTRING(x)                                                \
   decltype( pegtl::internal::ct_to_istring( PEGTL_STRING(x)() ) )

#endif
