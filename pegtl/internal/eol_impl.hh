// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_EOL_IMPL_HH
#define PEGTL_INTERNAL_EOL_IMPL_HH

#include <utility>

#include "../eol_mode.hh"

namespace pegtl
{
   namespace internal
   {
      using eol_pair = std::pair< bool, std::size_t >;

      template< eol_mode EOL > struct eol_impl;

      template<> struct eol_impl< eol_mode::LF_ONLY >
      {
         template< typename Input >
         static eol_pair match( Input & in )
         {
            eol_pair p = { false, in.size( 1 ) };
            if ( p.second ) {
               if ( in.peek_char() == '\n' ) {
                  in.bump_to_next_line();
                  p.first = true;
               }
            }
            return p;
         }
      };

      template<> struct eol_impl< eol_mode::CR_ONLY >
      {
         template< typename Input >
         static eol_pair match( Input & in )
         {
            eol_pair p = { false, in.size( 1 ) };
            if ( p.second ) {
               if ( in.peek_char() == '\r' ) {
                  in.bump_to_next_line();
                  p.first = true;
               }
            }
            return p;
         }
      };

      template<> struct eol_impl< eol_mode::CRLF_ONLY >
      {
         template< typename Input >
         static eol_pair match( Input & in )
         {
            eol_pair p = { false, in.size( 2 ) };
            if ( p.second > 1 ) {
               if ( ( in.peek_char() == '\r' ) && ( in.peek_char( 1 ) == '\n' ) ) {
                  in.bump_to_next_line( 2 );
                  p.first = true;
               }
            }
            return p;
         }
      };

      template<> struct eol_impl< eol_mode::LF_WITH_CRLF >
      {
         template< typename Input >
         static eol_pair match( Input & in )
         {
            eol_pair p = { false, in.size( 2 ) };
            if ( p.second ) {
               const auto a = in.peek_char();
               if ( a == '\n' ) {
                  in.bump_to_next_line();
                  p.first = true;
               }
               else if ( ( a == '\r' ) && ( p.second > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) {
                  in.bump_to_next_line( 2 );
                  p.first = true;
               }
            }
            return p;
         }
      };

      template<> struct eol_impl< eol_mode::CR_WITH_CRLF >
      {
         template< typename Input >
         static eol_pair match( Input & in )
         {
            eol_pair p = { false, in.size( 2 ) };
            if ( p.second ) {
               if ( in.peek_char() == '\r' ) {
                  in.bump_to_next_line( 1 + ( ( p.second > 1 ) && ( in.peek_char( 1 ) == '\n' ) ) );
                  p.first = true;
               }
            }
            return p;
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
