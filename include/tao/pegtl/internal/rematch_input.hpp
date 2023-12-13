// Copyright (c) 2019-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_REMATCH_INPUT_HPP
#define TAO_PEGTL_INTERNAL_REMATCH_INPUT_HPP

#include <cstddef>

#include "../config.hpp"
#include "../pointer_position.hpp"

#include "has_eol_rule.hpp"
#include "input_with_fakes.hpp"
#include "input_with_peeks.hpp"
#include "input_with_start.hpp"
#include "rewind_guard.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Guard, typename Input >
   class rematch_view_input
   {
   public:
      using base_t = input_with_peeks< input_with_fakes< input_with_start< typename Input::data_t > > >;
      using data_t = typename Input::data_t;
      using error_position_t = typename Input::error_position_t;
      using rewind_position_t = pointer_position< data_t >;

      rematch_view_input( Guard& m, Input& in )
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

      template< typename Rule >
      void consume( const std::size_t count ) noexcept
      {
         m_current += count;
      }

      template< rewind_mode M >
      [[nodiscard]] auto make_rewind_guard() noexcept
      {
         return rewind_guard< M, rematch_view_input >( this );
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
      : input_with_peeks< input_with_fakes< rematch_view_input< Guard, Input > > >
   {
      using input_with_peeks< input_with_fakes< rematch_view_input< Guard, Input > > >::input_with_peeks;
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
