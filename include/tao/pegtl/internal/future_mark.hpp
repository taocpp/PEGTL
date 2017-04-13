// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_FUTURE_MARK_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_FUTURE_MARK_HPP

#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< rewind_mode M >
         class future_mark
         {
         public:
            static constexpr rewind_mode next_rewind_mode = M;

            explicit future_mark( const char*& )
            {
            }

            future_mark( future_mark&& ) noexcept
            {
            }

            future_mark( const future_mark& ) = delete;
            void operator=( const future_mark& ) = delete;

            bool operator()( const bool result ) const noexcept
            {
               return result;
            }
         };

         template<>
         class future_mark< rewind_mode::REQUIRED >
         {
         public:
            static constexpr rewind_mode next_rewind_mode = rewind_mode::ACTIVE;

            explicit future_mark( const char*& i ) noexcept
               : m_saved( i ),
                 m_input( &i )
            {
            }

            future_mark( future_mark&& i ) noexcept
               : m_saved( i.m_saved ),
                 m_input( i.m_input )
            {
               i.m_input = nullptr;
            }

            ~future_mark()
            {
               if( m_input != nullptr ) {
                  ( *m_input ) = m_saved;
               }
            }

            future_mark( const future_mark& ) = delete;
            void operator=( const future_mark& ) = delete;

            bool operator()( const bool result ) noexcept
            {
               if( result ) {
                  m_input = nullptr;
                  return true;
               }
               return false;
            }

            const char* begin() const noexcept
            {
               return m_saved;
            }

            const char* iterator() const noexcept
            {
               return m_saved;
            }

         private:
            const char* m_saved;
            const char** m_input;
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
