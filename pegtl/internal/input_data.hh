// Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_INPUT_DATA_HH
#define PEGTL_INTERNAL_INPUT_DATA_HH

#include <cstdlib>

namespace pegtl
{
   namespace internal
   {
      struct input_data
      {
         input_data( const std::size_t line, const std::size_t byte_in_line, const char * begin, const char * end, const char * source )
               : line( line ),
                 byte_in_line( byte_in_line ),
                 begin( begin ),
                 end( end ),
                 source( source )
         { }

         std::size_t line;
         std::size_t byte_in_line;

         const char * begin;
         const char * end;
         const char * source;

         void bump( const std::size_t count )
         {
            for ( std::size_t i = 0; i < count; ++i ) {
               if ( begin[ i ] == '\n' ) {
                  ++line;
                  byte_in_line = 0;
               }
               else {
                  ++byte_in_line;
               }
            }
            begin += count;
         }

         void bump_in_this_line( const std::size_t count )
         {
            begin += count;
            byte_in_line += count;
         }

         void bump_to_next_line( const std::size_t count )
         {
            ++line;
            begin += count;
            byte_in_line = 0;
         }
      };

   } // internal

} // pegtl

#endif
