// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REWIND_GUARD_HPP
#define TAO_PEGTL_INTERNAL_REWIND_GUARD_HPP

#include <type_traits>
#include <utility>

#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< rewind_mode, typename ParseInput >
   class rewind_guard;

   template< typename ParseInput >
   class rewind_guard< rewind_mode::optional, ParseInput >
   {
   public:
      explicit constexpr rewind_guard( ParseInput* /*unused*/ ) noexcept
      {}

      rewind_guard( rewind_guard&& ) = delete;
      rewind_guard( const rewind_guard& ) = delete;

      void operator=( rewind_guard&& ) = delete;
      void operator=( const rewind_guard& ) = delete;

      [[nodiscard]] constexpr bool operator()( const bool result ) const noexcept
      {
         return result;
      }
   };

   template< typename ParseInput >
   class rewind_guard< rewind_mode::required, ParseInput >
   {
   public:
      explicit rewind_guard( ParseInput* in ) noexcept
         : m_input( in ),
           m_saved( in->rewind_position() )
      {}

      rewind_guard( rewind_guard&& ) = delete;
      rewind_guard( const rewind_guard& ) = delete;

      ~rewind_guard()
      {
         if( active() ) {
            rewind_restore();
         }
      }

      void operator=( rewind_guard&& ) = delete;
      void operator=( const rewind_guard& ) = delete;

      [[nodiscard]] bool active() const noexcept
      {
         return m_input != nullptr;
      }

      [[nodiscard]] bool canceled() const noexcept
      {
         return m_input == nullptr;
      }

      [[nodiscard]] bool operator()( const bool result ) noexcept
      {
         if( result ) {
            rewind_cancel();
            return true;
         }
         return false;
      }

      void rewind_cancel() noexcept
      {
         m_input = nullptr;
      }

      void rewind_restore() const noexcept
      {
         m_input->rewind_to_position( m_saved );
      }

      [[nodiscard]] const auto* current() const noexcept
      {
         return m_input->previous( m_saved );
      }

      [[nodiscard]] const auto& rewind_position() const noexcept
      {
         return m_saved;
      }

      [[nodiscard]] decltype( auto ) current_position() const
      {
         return m_input->previous_position( m_saved );
      }

   private:
      ParseInput* m_input;
      using saved_position_t = std::decay_t< decltype( std::declval< ParseInput >().rewind_position() ) >;
      saved_position_t m_saved;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
