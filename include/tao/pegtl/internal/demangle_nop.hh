// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_DEMANGLE_NOP_HH
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_DEMANGLE_NOP_HH

#include <string>

#include "../config.hh"

namespace TAOCPP_PEGTL_NAMESPACE
{
   namespace internal
   {
      inline std::string demangle( const char * symbol )
      {
         return symbol;
      }

   } // namespace internal

} // namespace TAOCPP_PEGTL_NAMESPACE

#endif
