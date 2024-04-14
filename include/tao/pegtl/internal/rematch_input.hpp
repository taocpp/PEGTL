// Copyright (c) 2019-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REMATCH_INPUT_HPP
#define TAO_PEGTL_INTERNAL_REMATCH_INPUT_HPP

#include <cstddef>

#include "../config.hpp"
#include "../pointer_position.hpp"

#include "input_with_fakes.hpp"
#include "input_with_funcs.hpp"
#include "input_with_start.hpp"
#include "type_traits.hpp"
#include "view_input.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Guard, typename Input >
   class rematch_input_impl
   {
   public:
      using data_t = typename Input::data_t;
      using error_position_t = typename Input::error_position_t;
      // offset_position_t
      using rewind_position_t = pointer_position< data_t >;

      rematch_input_impl( Guard& m, Input& in )
         : m_guard( m ),
           m_input( in ),
           m_current( m.current() )
      {}

      [[nodiscard]] bool empty() const noexcept
      {
         return m_current == m_input.current();
      }

      [[nodiscard]] std::size_t size() const noexcept
      {
         return m_input.current() - m_current;
      }

      [[nodiscard]] const data_t* start() const noexcept
      {
         return m_guard.current();
      }

      [[nodiscard]] const data_t* current( const std::size_t offset = 0 ) const noexcept
      {
         return m_current + offset;
      }

      [[nodiscard]] const data_t* previous( const rewind_position_t saved ) const noexcept
      {
         return m_current + saved.count;
      }

      [[nodiscard]] const data_t* end() const noexcept
      {
         return m_input.current();
      }

      void restart() noexcept
      {
         m_current = m_guard.current();
      }

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         m_current += count;
      }

      [[nodiscard]] auto rewind_position() const noexcept
      {
         return rewind_position_t( m_current );
      }

      void rewind_to_position( const rewind_position_t saved ) noexcept
      {
         m_current = saved.data;
      }

      [[nodiscard]] auto current_position() const
      {
         return previous_position( rewind_position_t( m_current ) );
      }

      [[nodiscard]] auto previous_position( const rewind_position_t saved ) const
      {
         return rematch_position( m_guard, m_input, saved );
      }

      void private_set_current( const data_t* current ) noexcept
      {
         m_current = current;
      }

   protected:
      const Guard& m_guard;
      const Input& m_input;

      const data_t* m_current;
   };

   template< typename Guard, typename Input, bool = has_eol_rule< Input > >
   struct rematch_input;

   template< typename Guard, typename Input >
   struct rematch_input< Guard, Input, false >
      : input_with_fakes< input_with_funcs< rematch_input_impl< Guard, Input > > >
   {
      using input_with_fakes< input_with_funcs< rematch_input_impl< Guard, Input > > >::input_with_fakes;
   };

   template< typename Guard, typename Input >
   struct rematch_input< Guard, Input, true >
      : rematch_input< Guard, Input, false >
   {
      using eol_rule = typename Input::eol_rule;

      using rematch_input< Guard, Input, false >::rematch_input;
   };

   template< typename Guard, typename Input >
   rematch_input( Guard&, Input& ) -> rematch_input< Guard, Input >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
