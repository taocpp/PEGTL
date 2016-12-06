// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_POSITION_INFO_HH
#define PEGTL_POSITION_INFO_HH

#include <string>
#include <sstream>
#include <ostream>
#include <cstdlib>

#include "internal/input_data.hh"

namespace pegtl
{
   struct position_info
   {
      explicit
      position_info( const internal::input_data & d )
            : byte( d.byte ),
              line( d.line ),
              byte_in_line( d.byte_in_line ),
              source( d.source )
      { }

      std::size_t byte;
      std::size_t line;
      std::size_t byte_in_line;
      std::string source;
   };

   inline std::ostream & operator<< ( std::ostream & o, const position_info & p )
   {
      return o << p.source << ':' << p.line << ':' << p.byte_in_line << '(' << p.byte << ')';
   }

   inline std::string to_string( const position_info & p )
   {
      std::ostringstream o;
      o << p;
      return o.str();
   }

} // namespace pegtl

#endif
