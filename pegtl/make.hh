// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_MAKE_HH
#define PEGTL_MAKE_HH

#include "ascii.hh"
#include "rules.hh"

namespace pegtl
{
   namespace internal
   {
     template< std::size_t... Ns >
     struct indices
     {
       using next = indices< Ns..., sizeof...( Ns ) >;
     };

     template< std::size_t N >
     struct make_indices
     {
       using type = typename make_indices< N - 1 >::type::next;
     };

     template<>
     struct make_indices< 0 >
     {
       using type = indices<>;
     };

     constexpr std::size_t length( const char* str )
     {
       return *str ? ( length( str + 1 ) + 1 ) : 0;
     }

     template< const char* str, typename = typename make_indices< length( str ) >::type >
     struct make;

     template< const char* str, std::size_t ... Is >
     struct make< str, indices< Is... > >
     {
       using one_t = pegtl::one< str[ Is ]... >;
       using string_t = pegtl::string< str[ Is ]... >;
       using istring_t = pegtl::istring< str[ Is ]... >;
     };

   } // internal

   template< const char* str >
   using make_one = typename internal::make< str >::one_t;

   template< const char* str >
   using make_string = typename internal::make< str >::string_t;

   template< const char* str >
   using make_istring = typename internal::make< str >::istring_t;

} // pegtl

#endif
