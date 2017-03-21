// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_DEMANGLE_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_DEMANGLE_HH

#include <string>
#include <typeinfo>

#include "../config.hh"

#if defined(__GLIBCXX__)
#include "demangle_cxxabi.hh"
#elif defined(__has_include)
#if __has_include(<cxxabi.h>)
#include "demangle_cxxabi.hh"
#else
#include "demangle_nop.hh"
#endif
#else
#include "demangle_nop.hh"
#endif

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename T >
      std::string demangle()
      {
         return demangle( typeid( T ).name() );
      }

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
