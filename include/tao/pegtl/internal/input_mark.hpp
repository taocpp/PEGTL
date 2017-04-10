// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_MARK_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_MARK_HPP

#include "../config.hpp"
#include "../rewind_mode.hpp"

#include "iterator.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< rewind_mode M >
         class input_mark
         {
         public:
            static constexpr rewind_mode next_rewind_mode = M;

            explicit input_mark( const internal::iterator& )
            {
            }

            input_mark( input_mark&& ) noexcept
            {
            }

            input_mark( const input_mark& ) = delete;
            void operator=( const input_mark& ) = delete;

            bool operator()( const bool result ) noexcept
            {
               return result;
            }
         };

         template<>
         class input_mark< rewind_mode::REQUIRED >
         {
         public:
            static constexpr rewind_mode next_rewind_mode = rewind_mode::ACTIVE;

            explicit input_mark( internal::iterator& i ) noexcept
               : m_saved( i ),
                 m_input( &i )
            {
            }

            input_mark( input_mark&& i ) noexcept
               : m_saved( i.m_saved ),
                 m_input( i.m_input )
            {
               i.m_input = nullptr;
            }

            ~input_mark() noexcept
            {
               if( m_input != nullptr ) {
                  ( *m_input ) = m_saved;
               }
            }

            input_mark( const input_mark& ) = delete;
            void operator=( const input_mark& ) = delete;

            bool operator()( const bool result ) noexcept
            {
               if( result ) {
                  m_input = nullptr;
                  return true;
               }
               return false;
            }

            const internal::iterator& iterator() const noexcept
            {
               return m_saved;
            }

         private:
            const internal::iterator m_saved;
            internal::iterator* m_input;
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
