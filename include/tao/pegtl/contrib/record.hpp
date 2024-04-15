// Copyright (c) 2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_RECORD_HPP
#define TAO_PEGTL_CONTRIB_RECORD_HPP

#include <cstddef>
#include <iostream>
#include <ostream>
#include <type_traits>

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../inputs.hpp"
#include "../match.hpp"
#include "../normal.hpp"
#include "../rewind_mode.hpp"

#include "dispatch.hpp"
#include "input_with_offset.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename ParseInput >
   using record_input = input_with_offset< internal::text_view_alias< typename ParseInput::eol_rule, typename ParseInput::data_t, void, void > >;

   template< typename ParseInput >
   struct record_value
   {
      using data_t = typename ParseInput::data_t;
      using position_t = typename ParseInput::error_position_t;

      record_value( const position_t& pos, const data_t* data, const std::size_t size, const std::string_view r )
         : input( pos, data, size ),
           rule( r )
      {}

      record_input< ParseInput > input;
      std::string_view rule;
   };

   template< typename ParseInput >
   using record_vector = std::vector< record_value< ParseInput > >;

   namespace internal
   {
      template< typename ParseInput >
      struct record_state
      {
         record_vector< ParseInput > samples;
      };

      template< typename Rule >
      struct record_action
      {
         template< typename ActionInput, typename ParseInput >
         static void apply( const ActionInput& in, record_state< ParseInput >& rs )
         {
            rs.samples.emplace_back( in.current_position(), in.current(), in.size(), demangle< Rule >() );
         }
      };

      template< typename RewindGuard, typename RecordValue >
      class record_control_guard
         : public RewindGuard
      {
      public:
         record_control_guard( RewindGuard&& rg, std::vector< RecordValue >& rs )
            : RewindGuard( std::move( rg ) ),
              m_samples( rs ),
              m_size( rs.size() )
         {}

         record_control_guard( record_control_guard&& ) = delete;
         record_control_guard( const record_control_guard& ) = delete;

         void operator=( record_control_guard&& ) = delete;
         void operator=( const record_control_guard& ) = delete;

         ~record_control_guard()
         {
            if( this->active() ) {
               // assert( m_state.samples.size() >= m_size );
               m_samples.erase( m_samples.begin() + m_size, m_samples.end() );
            }
         }

      private:
         std::vector< RecordValue >& m_samples;
         const std::size_t m_size;
      };

      template< typename RewindGuard, typename RecordValue >
      record_control_guard( RewindGuard&&, std::vector< RecordValue >& ) -> record_control_guard< RewindGuard, RecordValue >;

      template< template< typename... > class Base >
      struct record_control
      {
         template< typename Rule >
         struct type
            : Base< Rule >
         {
            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control,
                      typename ParseInput,
                      typename... States >
            static auto guard( ParseInput& in, record_state< ParseInput >& st )
            {
               if constexpr( M == rewind_mode::optional ) {
                  return Base< Rule >::template guard< A, M, Action, Control >( in, st );
               }
               else {
                  return record_control_guard( Base< Rule >::template guard< A, M, Action, Control >( in, st ), st.samples );
               }
            }
         };
      };

   }  // namespace internal

   template< typename... Rules >
   struct record
   {
      using clause = clause2< internal::record_action, Rules... >;

      template< typename Rule >
      using actions = internal::clauses_to_action_t< Rule, clause >;

      template< typename Rule,
                template< typename... >
                class Control = normal,
                typename ParseInput >
      [[nodiscard]] static auto parse( ParseInput&& in )
      {
         internal::record_state< std::decay_t< ParseInput > > state;
         if( !TAO_PEGTL_NAMESPACE::match< Rule, apply_mode::action, rewind_mode::optional, actions, internal::record_control< Control >::template type >( in, state ) ) {
            state.samples.clear();
         }
         return std::move( state.samples );
      }
   };

   template< typename ParseInput >
   std::ostream& operator<< ( std::ostream& o, const record_value< ParseInput >& r )
   {
      return o << r.rule << '@' << r.input.direct_offset() << " '" << r.input.string_view() << "'" << std::endl;
   }

   template< typename ParseInput >
   std::ostream& operator<< ( std::ostream& o, const record_vector< ParseInput >& v )
   {
      for( const auto& r : v ) {
         o << r;
      }
      return o;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
