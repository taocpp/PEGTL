// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYZE_HH
#define PEGTL_ANALYZE_HH

#include "config.hh"
#include "analysis/analyze_cycles.hh"

namespace PEGTL_NAMESPACE
{
   template< typename Rule >
   std::size_t analyze( const bool verbose = true )
   {
      return analysis::analyze_cycles< Rule >( verbose ).problems();
   }

} // namespace PEGTL_NAMESPACE

#endif
