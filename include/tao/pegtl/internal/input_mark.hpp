// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_MARK_HPP
#define TAOCPP_PEGTL_INCLUDE_INTERNAL_INPUT_MARK_HPP

#include "../config.hpp"
#include "../count_data.hpp"
#include "../rewind_mode.hpp"

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

            explicit input_mark( const count_data& )
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

            explicit input_mark( count_data& i ) noexcept
               : m_count( i ),
                 m_input( &i )
            {
            }

            input_mark( input_mark&& i ) noexcept
               : m_count( i.m_count ),
                 m_input( i.m_input )
            {
               i.m_input = nullptr;
            }

            ~input_mark() noexcept
            {
               if( m_input != nullptr ) {
                  ( *m_input ) = m_count;
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

            const char* begin() const noexcept
            {
               return m_count.data;
            }

            const count_data& count() const noexcept
            {
               return m_count;
            }

         private:
            const count_data m_count;
            count_data* m_input;
         };

      }  // namespace internal

   }  // namespace TAOCPP_PEGTL_NAMESPACE

}  // namespace tao

#endif
