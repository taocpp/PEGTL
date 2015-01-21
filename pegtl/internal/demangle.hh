// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DEMANGLE_HH
#define PEGTL_INTERNAL_DEMANGLE_HH

#include <typeinfo>

#if __GNUC__
// GCC and Clang both define __GNUC__
#include "demangle_gcc.hh"
#else
#include "demangle_nop.hh"
#endif

namespace pegtl
{
   namespace internal
   {
      template< typename T >
      std::string demangle()
      {
         return demangle( typeid( T ).name() );
      }

   } // internal

} // pegtl

#endif
