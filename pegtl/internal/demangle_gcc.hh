// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DEMANGLE_GCC_HH
#define PEGTL_INTERNAL_DEMANGLE_GCC_HH

#include <string>
#include <memory>
#include <cxxabi.h>

namespace pegtl
{
   namespace internal
   {
      struct free_deleter
      {
         void operator() ( void * p ) const
         {
            if ( p ) {
               ::free( p );
            }
         }
      };

      inline std::string demangle( const char * symbol )
      {
         int s = -1;
         std::shared_ptr< const char > p( abi::__cxa_demangle( symbol, 0, 0, & s ), free_deleter() );
         return std::string( ( p && ! s ) ? p.get() : symbol );
      }

   } // internal

} // pegtl

#endif
