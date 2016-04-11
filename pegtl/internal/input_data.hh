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
         input_data( const std::size_t line, const std::size_t column, const char * begin, const char * end, const char * source, const input_data * from = nullptr )
               : line( line ),
                 column( column ),
                 begin( begin ),
                 end( end ),
                 source( source ),
                 from( from )
         { }

         std::size_t line;
         std::size_t column;

         const char * begin;
         const char * end;
         const char * source;

         const input_data * from;

         void bump( const size_t count )
         {
            for ( std::size_t i = 0; i < count; ++i )
            {
               switch ( * begin ) {
                  case '\n':
                     ++line;
                     column = 0;
                     break;
                  default:
                     ++column;
                     break;
               }
               ++begin;
            }
         }

         void bump_in_line( const std::size_t count )
         {
            begin += count;
            column += count;
         }

         void bump_next_line( const std::size_t count )
         {
            ++line;
            begin += count;
            column = 0;
         }
      };

   } // internal

} // pegtl

#endif
