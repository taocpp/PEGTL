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
      template< typename Rule >
      struct add_sample
      {
         template< typename ActionInput, typename ParseInput >
         static void apply( const ActionInput& in, record_vector< ParseInput >& rv )
         {
            rv.emplace_back( in.current_position(), in.current(), in.size(), demangle< Rule >() );
         }
      };

      struct verify_fail
         : maybe_nothing
      {
         template< typename Rule,
                   apply_mode A,
                   rewind_mode M,
                   template< typename... >
                   class Action,
                   template< typename... >
                   class Control,
                   typename ParseInput,
                   typename... States >
         [[nodiscard]] static bool match( ParseInput& in, record_vector< ParseInput >& rv )
         {
            const std::size_t size = rv.size();
            if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, rv ) ) {
               return true;
            }
            if( rv.size() > size ) {
               std::cerr << demangle< Rule >() << " on fail grow " << size << " -> " << rv.size() << std::endl;
            }
            return false;
         }
      };

   }  // namespace internal

   template< typename... Rules >
   struct record
   {
      using clause = clause2< internal::add_sample, Rules... >;

      template< typename Rule >
      using actions = internal::clauses_to_action_t< Rule, clause >;

      template< typename Rule,
                apply_mode A = apply_mode::action,
                rewind_mode M = rewind_mode::optional,
                template< typename... >
                class Control = normal,
                typename ParseInput >
      [[nodiscard]] static auto parse( ParseInput&& in )
      {
         record_vector< std::decay_t< ParseInput > > samples;
         if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, actions, Control >( in, samples ) ) {
            return samples;
         }
         samples.clear();
         return samples;
      }
   };

   template< typename... Rules >
   struct record2
   {
      using clause = clause2< internal::add_sample, Rules... >;
      using defaults = default1< internal::verify_fail, clause >;

      template< typename Rule >
      using actions = internal::clauses_to_action_t< Rule, clause, defaults >;

      template< typename Rule,
                apply_mode A = apply_mode::action,
                rewind_mode M = rewind_mode::optional,
                template< typename... >
                class Control = normal,
                typename ParseInput >
      [[nodiscard]] static auto parse( ParseInput&& in )
      {
         record_vector< std::decay_t< ParseInput > > samples;
         if( TAO_PEGTL_NAMESPACE::match< Rule, A, M, actions, Control >( in, samples ) ) {
            return samples;
         }
         samples.clear();
         return samples;
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
