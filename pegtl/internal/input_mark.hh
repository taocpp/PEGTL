// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_INTERNAL_INPUT_MARK_HH
#define PEGTL_INTERNAL_INPUT_MARK_HH

#include "../config.hh"
#include "../count_data.hh"
#include "../rewind_mode.hh"

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< rewind_mode M > class input_mark
      {
      public:
         static PEGTL_CONSTEXPR rewind_mode next_rewind_mode = M;

         explicit
         input_mark( const count_data & )
         { }

         input_mark( input_mark && ) PEGTL_NOEXCEPT
         { }

         input_mark( const input_mark & ) = delete;
         void operator= ( const input_mark & ) = delete;

         bool operator() ( const bool result ) noexcept
         {
            return result;
         }
      };

      template<> class input_mark< rewind_mode::REQUIRED >
      {
      public:
         static PEGTL_CONSTEXPR rewind_mode next_rewind_mode = rewind_mode::ACTIVE;

         explicit
         input_mark( count_data & i ) PEGTL_NOEXCEPT
               : m_count( i ),
                 m_input( & i )
         { }

         input_mark( input_mark && i ) PEGTL_NOEXCEPT
               : m_count( i.m_count ),
                 m_input( i.m_input )
         {
            i.m_input = nullptr;
         }

         ~input_mark() noexcept
         {
            if ( m_input != nullptr ) {
               ( * m_input ) = m_count;
            }
         }

         input_mark( const input_mark & ) = delete;
         void operator= ( const input_mark & ) = delete;

         bool operator() ( const bool result ) noexcept
         {
            if ( result ) {
               m_input = nullptr;
               return true;
            }
            return false;
         }

         std::size_t byte() const noexcept
         {
            return m_count.byte;
         }

         std::size_t line() const noexcept
         {
            return m_count.line;
         }

         std::size_t byte_in_line() const noexcept
         {
            return m_count.byte_in_line;
         }

         const char * begin() const noexcept
         {
            return m_count.data;
         }

         const count_data & count() const noexcept
         {
            return m_count;
         }

      private:
         const count_data m_count;
         count_data * m_input;
      };

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
