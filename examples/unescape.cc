// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include <string>
#include <cassert>
#include <iostream>

#include <pegtl.hh>

namespace unescape
{
   namespace utility
   {
      inline bool utf8_append_ucs4( std::string & string, const unsigned ucs4 )
      {
         if ( ucs4 < 0x80 ) {
            char tmp = ucs4 & 0xff;
            string.append( & tmp, sizeof( tmp ) );
            return true;
         }
         else if ( ucs4 < 0x7ff ) {
            char tmp[] = { char( ( ( ucs4 & 0x7c0 ) >> 6 ) | 0xc0 ),
                           char( ( ( ucs4 & 0x03f )      ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
            return true;
         }
         else if ( ucs4 < 0xffff ) {
            char tmp[] = { char( ( ( ucs4 & 0xf000 ) >> 12 ) | 0xe0 ),
                           char( ( ( ucs4 & 0x0fc0 ) >> 6  ) | 0x80 ),
                           char( ( ( ucs4 & 0x003f )       ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
            return true;
         }
         else if ( ucs4 < 0x10ffff ) {
            char tmp[] = { char( ( ( ucs4 & 0x1c0000 ) >> 18 ) | 0xf0 ),
                           char( ( ( ucs4 & 0x03f000 ) >> 12 ) | 0x80 ),
                           char( ( ( ucs4 & 0x000fc0 ) >> 6  ) | 0x80 ),
                           char( ( ( ucs4 & 0x00003f )       ) | 0x80 ) };
            string.append( tmp, sizeof( tmp ) );
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
         assert( false );
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

   } // utility

   namespace grammar
   {
      // NOTE: The C-standard requires rep_min rather than rep for 'x'.
      struct escaped_x : pegtl::if_must< pegtl::one< 'x' >, pegtl::rep_min< 2, pegtl::xdigit > > {};
      struct escaped_u : pegtl::if_must< pegtl::one< 'u' >, pegtl::rep< 4, pegtl::xdigit > > {};
      struct escaped_U : pegtl::if_must< pegtl::one< 'U' >, pegtl::rep< 8, pegtl::xdigit > > {};
      struct escaped_c : pegtl::one< '\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v' > {};

      struct escaped : pegtl::sor< escaped_x,
                                   escaped_u,
                                   escaped_U,
                                   escaped_c > {};

      struct character : pegtl::if_must_else< pegtl::one< '\\' >, escaped, pegtl::any > {};
      struct literal : pegtl::if_must< pegtl::one< '"' >, pegtl::until< pegtl::one< '"' >, character > > {};
      struct padded : pegtl::seq< pegtl::pad< literal, pegtl::blank >, pegtl::eof > {};

   } // grammar

   namespace actions
   {
      struct state
      {
         std::string unescaped;
      };

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
         static char apply_one( const char c, const pegtl::one< Qs ... > * )
         {
            static_assert( sizeof ... ( Qs ) == sizeof ... ( Rs ), "Size mismatch between escaped characters and their mappings." );
            return apply_two( c, { Qs ... }, { Rs ... } );
         }

         static char apply_two( const char c, const std::initializer_list< char > & q, const std::initializer_list< char > & r )
         {
            for ( size_t i = 0; i < q.size(); ++i ) {
               if ( * ( q.begin() + i ) == c ) {
                  return * ( r.begin() + i );
               }
            }
            assert( false );  // This can only be triggered when this action is attached to the wrong rule.
         }
      };

      struct unescape_u
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            assert( ! in.empty() );
            utility::utf8_append_ucs4( st.unescaped, utility::unhex_string< unsigned >( in.begin() + 1, in.end() ) );
         }
      };

      struct unescape_x
      {
         template< typename Input, typename State >
         static void apply( const Input & in, State & st )
         {
            assert( ! in.empty() );
            st.unescaped += utility::unhex_string< char >( in.begin() + 1, in.end() );
         }
      };

      template< typename Rule > struct unescape : pegtl::nothing< Rule > {};

      template<> struct unescape< pegtl::any > : append_all {};

      template<> struct unescape< grammar::escaped_x > : unescape_x {};
      template<> struct unescape< grammar::escaped_u > : unescape_u {};
      template<> struct unescape< grammar::escaped_U > : unescape_u {};

      template<> struct unescape< grammar::escaped_c > : unescape_c< grammar::escaped_c, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v' > {};

   } // actions

} // unescape

int main( int argc, char * argv[] )
{
   for ( int i = 1; i < argc; ++i ) {
      unescape::actions::state s;
      pegtl::parse< unescape::grammar::padded, unescape::actions::unescape >( i, argv, s );
      std::cout << "argv[ " << ( i + 1 ) << " ] = " << s.unescaped << std::endl;
   }
   return 0;
}
