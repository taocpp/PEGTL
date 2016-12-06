// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PARSE_ERROR_HH
#define PEGTL_PARSE_ERROR_HH

#include <vector>
#include <stdexcept>

#include "position_info.hh"

namespace pegtl
{
   template< typename Pos >
   struct basic_parse_error
         : public std::runtime_error
   {
      basic_parse_error( const std::string & message, std::vector< Pos > && in_positions )
            : std::runtime_error( message ),
              positions( std::move( in_positions ) )
      { }

      template< typename Input >
      basic_parse_error( const std::string & message, const Input & in )
            : std::runtime_error( to_string( in.position() ) + ": " + message ),
              positions( 1, in.position() )
      { }

      using position_t = Pos;

      std::vector< Pos > positions;
   };

   using parse_error = basic_parse_error< position_info >;

} // namespace pegtl

#endif
