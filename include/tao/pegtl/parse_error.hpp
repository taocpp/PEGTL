// Copyright (c) 2014-2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_PARSE_ERROR_HPP
#define TAO_PEGTL_PARSE_ERROR_HPP

#include <stdexcept>
#include <utility>
#include <vector>

#include "config.hpp"
#include "position.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   struct parse_error
      : public std::runtime_error
   {
      template< typename Msg >
      parse_error( Msg&& msg, const std::vector< position >& in_positions )
         : std::runtime_error( std::forward< Msg >( msg ) ),
           positions( in_positions )
      {
      }

      template< typename Msg >
      parse_error( Msg&& msg, std::vector< position >&& in_positions )
         : std::runtime_error( std::forward< Msg >( msg ) ),
           positions( std::move( in_positions ) )
      {
      }

      template< typename Msg >
      parse_error( Msg&& msg, const position& pos )
         : std::runtime_error( std::forward< Msg >( msg ) ),
           positions( 1, pos )
      {
      }

      template< typename Msg >
      parse_error( Msg&& msg, position&& pos )
         : std::runtime_error( std::forward< Msg >( msg ) )
      {
         positions.emplace_back( std::move( pos ) );
      }

      template< typename Msg, typename Input >
      parse_error( Msg&& msg, const Input& in )
         : parse_error( std::forward< Msg >( msg ), in.position() )
      {
      }

      std::vector< position > positions;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
