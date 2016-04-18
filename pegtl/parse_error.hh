// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PARSE_ERROR_HH
#define PEGTL_PARSE_ERROR_HH

#include <vector>
#include <stdexcept>

#include "position_info.hh"

namespace pegtl
{
   struct parse_error
         : public std::runtime_error
   {
      parse_error( const std::string & message, std::vector< position_info > && positions )
            : std::runtime_error( message ),
              positions( std::move( positions ) )
      { }

      template< typename Input >
      parse_error( const std::string & message, const Input & in )
            : std::runtime_error( to_string( position_info( in ) ) + ": " + message ),
              positions( 1, position_info( in ) )
      { }

      std::vector< position_info > positions;
   };

} // pegtl

#endif
