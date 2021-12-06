// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REWIND_GUARD_HPP
#define TAO_PEGTL_INTERNAL_REWIND_GUARD_HPP

#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Frobnicator, rewind_mode M >
   class [[nodiscard]] rewind_guard
   {
   public:
      static constexpr rewind_mode next_rewind_mode = M;

      explicit rewind_guard( const Frobnicator& /*unused*/ ) noexcept
      {}

      rewind_guard( const rewind_guard& ) = delete;
      rewind_guard( rewind_guard&& ) = delete;

      ~rewind_guard() = default;

      rewind_guard& operator=( const rewind_guard& ) = delete;
      rewind_guard& operator=( rewind_guard&& ) = delete;

      [[nodiscard]] bool operator()( const bool result ) const noexcept
      {
         return result;
      }
   };

   template< typename Frobnicator >
   class [[nodiscard]] rewind_guard< Frobnicator, rewind_mode::required >
   {
   public:
      static constexpr rewind_mode next_rewind_mode = rewind_mode::active;

      explicit rewind_guard( Frobnicator& i ) noexcept
         : m_saved( i ),
           m_input( &i )
      {}

      rewind_guard( const rewind_guard& ) = delete;
      rewind_guard( rewind_guard&& ) = delete;

      ~rewind_guard()
      {
         if( m_input != nullptr ) {
            ( *m_input ) = m_saved;
         }
      }

      rewind_guard& operator=( const rewind_guard& ) = delete;
      rewind_guard& operator=( rewind_guard&& ) = delete;

      [[nodiscard]] bool operator()( const bool result ) noexcept
      {
         if( result ) {
            m_input = nullptr;
            return true;
         }
         return false;
      }

      [[nodiscard]] const Frobnicator& frobnicator() const noexcept
      {
         return m_saved;
      }

   private:
      const Frobnicator m_saved;
      Frobnicator* m_input;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
