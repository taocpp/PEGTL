// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_POSITION_INFO_HH
#define PEGTL_POSITION_INFO_HH

#include <string>
#include <cstdlib>
#include <ostream>

#include "input.hh"

#include "internal/input_data.hh"

namespace pegtl
{
   struct position_info
   {
      explicit
      position_info( const input & in )
            : position_info( in.data() )
      { }

      explicit
      position_info( const internal::input_data & id )
            : source( id.source ),
              number( id.number ),
              offset( id.offset ),
              begin( id.begin )
      { }

      std::string source;
      std::size_t number;
      std::size_t offset;

      const char * begin;
   };

   inline std::ostream & operator<< ( std::ostream & o, const position_info & p )
   {
      return o << p.source << ':' << p.number << ':' << p.offset;
   }

} // pegtl

#endif
