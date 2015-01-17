// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ASSERTIONS_HH
#define PEGTL_ASSERTIONS_HH

#include "rules.hh"

namespace pegtl
{
   template< typename ... Rule >
   struct star< opt< Rule ... > >
   {
      static_assert( ! sizeof( star< opt< Rule ... > > ), "pegtl: illegal expression star< opt< rule > > (allows iteration without progress = infinite loop)" );
   };

   template< typename ... Rule >
   struct plus< opt< Rule ... > >
   {
      static_assert( ! sizeof( plus< opt< Rule ... > > ), "pegtl: illegal expression plus< opt< rule > > (allows iteration without progress = infinite loop)" );
   };

   template< typename ... Rule >
   struct star< star< Rule ... > >
   {
      static_assert( ! sizeof( star< star< Rule ... > > ), "pegtl: illegal expression star< star< rule > > (allows iteration without progress = infinite loop)" );
   };

   template< typename ... Rule >
   struct plus< star< Rule ... > >
   {
      static_assert( ! sizeof( plus< star< Rule ... > > ), "pegtl: illegal expression plus< star< rule > > (allows iteration without progress = infinite loop)" );
   };

} // pegtl

#endif
