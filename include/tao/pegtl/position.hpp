// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_POSITION_HPP
#define TAO_PEGTL_POSITION_HPP

#include <cstdlib>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "config.hpp"

#include "internal/iterator.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct position
   {
      position() noexcept {}

      position( position&& ) noexcept = default;
      position( const position& ) = default;

      position& operator=( position&& ) noexcept = default;
      position& operator=( const position& ) = default;

      template< typename T >
      position( const internal::iterator& in_iter, T&& in_source )
         : byte( in_iter.byte ),
           line( in_iter.line ),
           byte_in_line( in_iter.byte_in_line ),
           source( std::forward< T >( in_source ) )
      {
      }

      std::size_t byte = 0;
      std::size_t line = 0;
      std::size_t byte_in_line = 0;
      std::string source;
   };

   inline std::ostream& operator<<( std::ostream& o, const position& p )
   {
      return o << p.source << ':' << p.line << ':' << p.byte_in_line << '(' << p.byte << ')';
   }

   [[nodiscard]] inline std::string to_string( const position& p )
   {
      std::ostringstream o;
      o << p;
      return o.str();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
