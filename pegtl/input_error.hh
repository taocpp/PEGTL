// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INPUT_ERROR_HH
#define PEGTL_INPUT_ERROR_HH

#include <sstream>
#include <stdexcept>

namespace pegtl
{
   struct input_error
         : std::runtime_error
   {
      explicit
      input_error( const std::string & message )
            : std::runtime_error( message ),
              errorno( errno )
      { }

      int errorno;
   };

#define PEGTL_THROW_INPUT_ERROR( MeSSaGe )                              \
   do {                                                                 \
      std::ostringstream oss;                                           \
      oss << "pegtl: " << MeSSaGe << " errno " << errno;                \
      throw pegtl::input_error( oss.str() );                          \
   } while ( 1 )

} // pegtl

#endif
