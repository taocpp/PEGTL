// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef PEGTL_POSITION_INFO_HH
#define PEGTL_POSITION_INFO_HH

#include <string>
#include <sstream>
#include <ostream>
#include <cstdlib>

#include "config.hh"
#include "count_data.hh"

namespace PEGTL_NAMESPACE
{
   struct position_info
   {
      position_info( const count_data & in_data, const char * in_source )
            : byte( in_data.byte ),
              line( in_data.line ),
              byte_in_line( in_data.byte_in_line ),
              source( in_source )
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

} // namespace PEGTL_NAMESPACE

#endif
