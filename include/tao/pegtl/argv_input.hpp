// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_ARGV_INPUT_HPP
#define TAOCPP_PEGTL_INCLUDE_ARGV_INPUT_HPP

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>

#include "config.hpp"
#include "eol.hpp"
#include "memory_input.hpp"
#include "tracking_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         inline std::string make_argv_source( const std::size_t argn )
         {
            std::ostringstream os;
            os << "argv[" << argn << ']';
            return os.str();
         }

         struct argv_holder
         {
            const std::string argv_source;

            template< typename T >
            explicit argv_holder( T&& in_argv_source )
               : argv_source( std::forward< T >( in_argv_source ) )
            {
            }
         };

      }  // namespace internal

      template< typename Eol = lf_crlf_eol, tracking_mode P = tracking_mode::IMMEDIATE >
      struct argv_input
         : private internal::argv_holder,
           public memory_input< Eol, P >
      {
         template< typename T >
         argv_input( char** argv, const std::size_t argn, T&& in_source )
            : internal::argv_holder( std::forward< T >( in_source ) ),
              memory_input< Eol, P >( static_cast< const char* >( argv[ argn ] ), argv_source )
         {
         }

         argv_input( char** argv, const std::size_t argn )
            : argv_input( argv, argn, internal::make_argv_source( argn ) )
         {
         }
      };

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
