// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ERROR_MODE_HH
#define PEGTL_ERROR_MODE_HH

namespace pegtl
{
   enum class error_mode : bool
   {
      THROW = true,
      RETURN = false
   };

} // pegtl

#endif
