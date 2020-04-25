// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_TRACE2_HPP
#define TAO_PEGTL_CONTRIB_TRACE2_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <tuple>

#include "remove_first_state.hpp"
#include "shuffle_states.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../demangle.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"
#include "../rewind_mode.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct trace_state
      {
         std::ostream& m_os;
         std::size_t m_line = 0;
         TAO_PEGTL_NAMESPACE::position m_position;
         std::size_t m_indent = 6;
         bool m_unwind = false;

         trace_state( std::ostream& os, const TAO_PEGTL_NAMESPACE::position& p ) noexcept
            : m_os( os ), m_position( p )
         {
            m_os << std::left;
            ++m_position.byte;
            update( p );
         }

         void update( const TAO_PEGTL_NAMESPACE::position& p )
         {
            if( m_position != p ) {
               m_position = p;
               m_os << std::setw( m_indent ) << ' ' << "\033[1;34mposition\033[m " << p << '\n';
            }
         }
      };

      template< template< typename... > class Control >
      struct make_trace_control
      {
         template< typename Rule >
         struct control
            : remove_first_state< Control< Rule > >
         {
            template< apply_mode A,
                      rewind_mode M,
                      template< typename... >
                      class Action,
                      template< typename... >
                      class Control2,
                      typename ParseInput,
                      typename... States >
            [[nodiscard]] static bool match( ParseInput& in, States&&... st )
            {
               if constexpr( !control::enable ) {
                  return Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
               }

               auto& state = std::get< sizeof...( st ) - 1 >( std::tie( st... ) );
               const auto name = demangle< Rule >();
               const auto line = ++state.m_line;
               state.m_os << '#' << std::setw( state.m_indent - 1 ) << line << "\033[36m" << name << "\033[m\n";
               state.m_indent += 2;
               try {
                  const bool result = Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
                  state.m_unwind = false;
                  state.m_indent -= 2;
                  state.m_os << std::setw( state.m_indent ) << ' ';
                  if( result ) {
                     state.m_os << "\033[32msuccess\033[m";
                  }
                  else {
                     state.m_os << "\033[31mfailure\033[m";
                  }
                  if( state.m_line != line ) {
                     state.m_os << " #" << line << ' ' << "\033[37m" << name << "\033[m";
                  }
                  state.m_os << '\n';
                  state.update( in.position() );
                  return result;
               }
               catch( ... ) {
                  state.m_indent -= 2;
                  state.m_os << std::setw( state.m_indent ) << ' ';
                  if( state.m_unwind ) {
                     state.m_os << "\033[31munwind\033[m #" << line << " \033[37m" << name << "\033[m\n";
                  }
                  else {
                     state.m_os << "\033[1;31mraise\033[m #" << line << " \033[36m" << name << "\033[m\n";
                     state.m_unwind = true;
                  }
                  throw;
               }
            }

            template< template< typename... > class Action, typename Iterator, typename ParseInput, typename... States >
            static auto apply( const Iterator& begin, const ParseInput& in, const trace_state& state, States&&... st )
               -> decltype( Control< Rule >::template apply< Action >( begin, in, st... ) )
            {
               state.m_os << std::setw( static_cast< int >( state.m_indent - 2 ) ) << ' ' << "\033[1;36mapply\033[m\n";
               return Control< Rule >::template apply< Action >( begin, in, st... );
            }

            template< template< typename... > class Action, typename ParseInput, typename... States >
            static auto apply0( const ParseInput& in, const trace_state& state, States&&... st )
               -> decltype( Control< Rule >::template apply0< Action >( in, st... ) )
            {
               state.m_os << std::setw( static_cast< int >( state.m_indent - 2 ) ) << ' ' << "\033[1;36mapply0\033[m\n";
               return Control< Rule >::template apply0< Action >( in, st... );
            }
         };

         template< typename Rule >
         using type = rotate_states_right< control< Rule > >;
      };

   }  // namespace internal

   struct enable_trace
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
      [[nodiscard]] static bool match( ParseInput& in, States&&... st )
      {
         internal::trace_state state( std::cerr, in.position() );
         return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, internal::make_trace_control< Control >::template type >( in, st..., state );
      }
   };

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool trace( std::ostream& os, ParseInput&& in, States&&... st )
   {
      internal::trace_state state( os, in.position() );
      return parse< Rule, Action, internal::make_trace_control< Control >::template type >( in, st..., state );
   }

   template< typename Rule,
             template< typename... > class Action = nothing,
             template< typename... > class Control = normal,
             typename ParseInput,
             typename... States >
   bool trace( ParseInput&& in, States&&... st )
   {
      return trace< Rule, Action, Control >( std::cerr, in, st... );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
