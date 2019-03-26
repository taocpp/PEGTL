// Copyright (c) 2017-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PARSE_ERROR_HPP
#define TAO_PEGTL_CONTRIB_PARSE_ERROR_HPP

#include <sstream>
#include <string>

#include "../config.hpp"
#include "../parse_error.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   [[nodiscard]] std::string to_string( const parse_error& e )
   {
      std::stringstream o;
      for( auto it = e.positions.rbegin(); it != e.positions.rend(); ++it ) {
         o << *it;
         o << ": ";
      }
      o << e.what();
      return o.str();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
