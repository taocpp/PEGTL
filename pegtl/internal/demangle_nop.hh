// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_DEMANGLE_NOP_HH
#define PEGTL_INTERNAL_DEMANGLE_NOP_HH

#include <string>

#include "../config.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      inline std::string demangle( const char * symbol )
      {
         return symbol;
      }

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
