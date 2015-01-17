// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PARSE_ERROR_HH
#define PEGTL_PARSE_ERROR_HH

#include <vector>
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "position_info.hh"
#include "internal/demangle.hh"

namespace pegtl
{
   struct parse_error
         : std::runtime_error
   {
      parse_error( const std::string & message, std::vector< position_info > && positions )
            : std::runtime_error( message ),
              positions( std::move( positions ) )
      { }

      std::vector< position_info > positions;
   };

#define PEGTL_THROW_PARSE_ERROR( RuLe, iNPuT )                          \
   do {                                                                 \
      std::vector< pegtl::position_info > positions;                  \
      for ( const auto * id = & iNPuT.data(); id; id = id->from ) {     \
         positions.push_back( position_info( * id ) );                  \
      }                                                                 \
      assert( ! positions.empty() );                                    \
      std::ostringstream oss;                                           \
      oss << "pegtl: parse error matching rule " << pegtl::internal::demangle< Rule >() << " reading " << positions[ 0 ]; \
      throw pegtl::parse_error( oss.str(), std::move( positions ) );  \
   } while ( 1 )

} // pegtl

#endif
