// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_INPUT_MARK_HH
#define PEGTL_INTERNAL_INPUT_MARK_HH

#include "input_data.hh"

namespace pegtl
{
   namespace internal
   {
      class input_mark
      {
      public:
         explicit
         input_mark( input_data & i )
               : m_saved( i ),
                 m_input( & i )
         { }

         input_mark( input_mark && i )
               : m_saved( i.m_saved ),
                 m_input( i.m_input )
         { }

         ~input_mark()
         {
            if ( m_input ) {
               ( * m_input ) = m_saved;
            }
         }

         input_mark( const input_mark & ) = delete;
         void operator= ( const input_mark & ) = delete;

         bool success()
         {
            m_input = 0;
            return true;
         }

         bool failure()
         {
            ( * m_input ) = m_saved;
            m_input = 0;
            return false;
         }

         bool operator() ( const bool result )
         {
            return result ? success() : failure();
         }

         const input_data & saved() const
         {
            return m_saved;
         }

         const input_data * input() const
         {
            return m_input;
         }

      private:
         const input_data m_saved;
         input_data * m_input;
      };

   } // internal

} // pegtl

#endif
