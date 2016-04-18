// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_POSITION_INFO_HH
#define PEGTL_POSITION_INFO_HH

#include <string>
#include <sstream>
#include <ostream>
#include <cstdlib>

namespace pegtl
{
   struct position_info
   {
      template< typename Input >
      explicit
      position_info( const Input & in )
            : line( in.line() ),
              byte_in_line( in.byte_in_line() ),
              source( in.source() )
      { }

      std::size_t line;
      std::size_t byte_in_line;
      std::string source;
   };

   inline std::ostream & operator<< ( std::ostream & o, const position_info & p )
   {
      return o << p.source << ':' << p.line << ':' << p.byte_in_line;
   }

   inline std::string to_string( const position_info & p )
   {
      std::ostringstream o;
      o << p;
      return o.str();
   }

} // pegtl

#endif
