// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_UNESCAPE_HH
#define PEGTL_CONTRIB_UNESCAPE_HH

#include <string>
#include <cassert>

#include <pegtl/ascii.hh>

namespace pegtl
{
   namespace unescape
   {
      struct state
      {
         std::string unescaped;
      };

      // Utility functions for the unescape actions.

      inline bool utf8_append_ucs4( std::string & string, const unsigned ucs4 )
      {
         if ( ucs4 <= 0x7f ) {
            string += char( ucs4 & 0xff );
            return true;
         }
         else if ( ucs4 <= 0x7ff ) {
            char tmp[] = { char( ( ( ucs4 & 0x7c0 ) >> 6 ) | 0xc0 ),
                           char( ( ( ucs4 & 0x03f )      ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
            return true;
         }
         else if ( ucs4 <= 0xffff ) {
            char tmp[] = { char( ( ( ucs4 & 0xf000 ) >> 12 ) | 0xe0 ),
                           char( ( ( ucs4 & 0x0fc0 ) >> 6  ) | 0x80 ),
                           char( ( ( ucs4 & 0x003f )       ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
            return true;
         }
         else if ( ucs4 <= 0x10ffff ) {
            char tmp[] = { char( ( ( ucs4 & 0x1c0000 ) >> 18 ) | 0xf0 ),
                           char( ( ( ucs4 & 0x03f000 ) >> 12 ) | 0x80 ),
                           char( ( ( ucs4 & 0x000fc0 ) >> 6  ) | 0x80 ),
                           char( ( ( ucs4 & 0x00003f )       ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
            return true;
         }
         return false;
      }

      template< typename I >
      I unhex_char( const char c )
      {
         switch ( c ) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
               return I( c - '0' );
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
               return I( c - 'a' + 10 );
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
               return I( c - 'A' + 10 );
         }
         assert( false );  // This function MUST only be called for characters matching pegtl::ascii::xdigit!
      }

      template< typename I >
      I unhex_string( const char * begin, const char * const end )
      {
         I r = 0;
         while ( begin != end ) {
            r <<= 4;
            r += unhex_char< I >( *begin++ );
         }
         return r;
      }

      // Actions for common unescape situations.

      struct append_all
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            st.unescaped.append( in.begin(), in.end() );
         }
      };

      template< typename T, char ... Rs >
      struct unescape_c
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            assert( in.size() == 1 );
            st.unescaped += apply_one( * in.begin(), static_cast< const T * >( nullptr ) );
         }

         template< char ... Qs >
         static char apply_one( const char c, const one< Qs ... > * )
         {
            static_assert( sizeof ... ( Qs ) == sizeof ... ( Rs ), "size mismatch between escaped characters and their mappings" );
            return apply_two( c, { Qs ... }, { Rs ... } );
         }

         static char apply_two( const char c, const std::initializer_list< char > & q, const std::initializer_list< char > & r )
         {
            for ( std::size_t i = 0; i < q.size(); ++i ) {
               if ( * ( q.begin() + i ) == c ) {
                  return * ( r.begin() + i );
               }
            }
            assert( false );  // This function MUST be called for a character matching T which must be pegtl::one< ... >.
         }
      };

      // See examples/unescape.cc to see why the following two actions
      // have the convenience of skipping the first input character...

      struct unescape_u
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            assert( ! in.empty() );  // First character MUST be present, usually 'u' or 'U'.
            utf8_append_ucs4( st.unescaped, unhex_string< unsigned >( in.begin() + 1, in.end() ) );
         }
      };

      struct unescape_x
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            assert( ! in.empty() );  // First character MUST be present, usually 'x'.
            st.unescaped += unhex_string< char >( in.begin() + 1, in.end() );
         }
      };

   } // unescape

} // pegtl

#endif
