// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_TRACER_HPP
#define TAO_PEGTL_CONTRIB_TRACER_HPP

#include <cstddef>
#include <iomanip>
#include <ostream>
#include <tuple>

#include "remove_first_state.hpp"
#include "shuffle_states.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../normal.hpp"
#include "../nothing.hpp"
#include "../parse.hpp"
#include "../rewind_mode.hpp"

#include "../internal/demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      struct trace_state
      {
         std::ostream& m_os;
         std::size_t m_line = 0;
         TAO_PEGTL_NAMESPACE::position m_position;
         std::size_t m_indent = 8;

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
               m_os << std::setw( m_indent ) << "------>" << p << '\n';
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
               const auto name = internal::demangle< Rule >();
               const auto line = ++state.m_line;
               state.m_os << std::setw( state.m_indent ) << line << "\033[34m" << name << "\033[m\n";
               state.m_indent += 2;
               try {
                  const bool result = Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
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
                  if( state.m_line != line ) {
                     state.m_os << "\033[31munwind\033[m #" << line << " \033[37m" << name << "\033[m\n";
                  }
                  else {
                     state.m_os << "\033[1;31mraise " << name << "\033[m\n";
                  }
                  throw;
               }
            }
         };

         template< typename Rule >
         using type = rotate_states_right< control< Rule > >;
      };

   }  // namespace internal

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

}  // namespace TAO_PEGTL_NAMESPACE

#endif
