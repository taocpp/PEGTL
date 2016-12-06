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
         input_data( const std::size_t in_byte, const std::size_t in_line, const std::size_t in_byte_in_line, const char * in_begin, const char * in_end, const char * in_source )
               : byte( in_byte ),
                 line( in_line ),
                 byte_in_line( in_byte_in_line ),
                 begin( in_begin ),
                 end( in_end ),
                 source( in_source )
         { }

         std::size_t byte;
         std::size_t line;
         std::size_t byte_in_line;

         const char * begin;
         const char * end;
         const char * source;

         void bump( const std::size_t count, const int ch )
         {
            for ( std::size_t i = 0; i < count; ++i ) {
               if ( begin[ i ] == ch ) {
                  ++line;
                  byte_in_line = 0;
               }
               else {
                  ++byte_in_line;
               }
            }
            begin += count;
            byte += count;
         }

         void bump_in_this_line( const std::size_t count )
         {
            byte += count;
            begin += count;
            byte_in_line += count;
         }

         void bump_to_next_line( const std::size_t count )
         {
            ++line;
            byte += count;
            begin += count;
            byte_in_line = 0;
         }
      };

   } // namespace internal

} // namespace pegtl

#endif
