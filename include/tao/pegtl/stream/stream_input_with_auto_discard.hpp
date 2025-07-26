// Copyright (c) 2022-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_STREAM_STREAM_INPUT_WITH_AUTO_DISCARD_HPP
#define TAO_PEGTL_STREAM_STREAM_INPUT_WITH_AUTO_DISCARD_HPP

#include <cstddef>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename StreamInput >
   class stream_input_with_auto_discard
      : public StreamInput
   {
   public:
      using StreamInput::StreamInput;

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         this->StreamInput::template consume< Rule >( count );
         auto_discard();
      }

      void inc_rewind_guards() noexcept
      {
         ++m_rewind_guards;
      }

      void dec_rewind_guards() noexcept
      {
         // assert( m_rewind_guards > 0 );
         --m_rewind_guards;
         auto_discard();
      }

   private:
      std::size_t m_rewind_guards = 0;

      void auto_discard() noexcept
      {
         if( m_rewind_guards == 0 ) {
            this->StreamInput::discard();
         }
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
