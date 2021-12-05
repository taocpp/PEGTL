// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_MARKER_HPP
#define TAO_PEGTL_INTERNAL_MARKER_HPP

#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Frobnicator, rewind_mode M >
   class [[nodiscard]] marker
   {
   public:
      static constexpr rewind_mode next_rewind_mode = M;

      explicit marker( const Frobnicator& /*unused*/ ) noexcept
      {}

      marker( const marker& ) = delete;
      marker( marker&& ) = delete;

      ~marker() = default;

      marker& operator=( const marker& ) = delete;
      marker& operator=( marker&& ) = delete;

      [[nodiscard]] bool operator()( const bool result ) const noexcept
      {
         return result;
      }
   };

   template< typename Frobnicator >
   class [[nodiscard]] marker< Frobnicator, rewind_mode::required >
   {
   public:
      static constexpr rewind_mode next_rewind_mode = rewind_mode::active;

      explicit marker( Frobnicator& i ) noexcept
         : m_saved( i ),
           m_input( &i )
      {}

      marker( const marker& ) = delete;
      marker( marker&& ) = delete;

      ~marker()
      {
         if( m_input != nullptr ) {
            ( *m_input ) = m_saved;
         }
      }

      marker& operator=( const marker& ) = delete;
      marker& operator=( marker&& ) = delete;

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
