// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_EOL_MODE_HH
#define PEGTL_EOL_MODE_HH

#include <cstdlib>

namespace pegtl
{
   enum class eol_mode : char
   {
      LF_ONLY,
      CR_ONLY,
      CRLF_ONLY,
      LF_WITH_CRLF,
      CR_WITH_CRLF
   };

   constexpr int eol_mode_to_int( const eol_mode m )
   {
      return ( ( m == eol_mode::CR_ONLY ) || ( m == eol_mode::CR_WITH_CRLF ) ) ? '\r' : '\n';
   }

} // namespace pegtl

#endif
