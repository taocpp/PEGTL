// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_TRACE2_HPP
#define TAO_PEGTL_CONTRIB_TRACE2_HPP

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <tuple>

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
   template< template< typename... > class Control, typename State >
   struct state_control
   {
      template< typename Rule >
      struct control
         : Control< Rule >
      {
         static constexpr bool enable = State::template enable< Rule >;

         template< typename ParseInput, typename... States >
         static void start( const ParseInput& in, State& state, States&&... st )
         {
            state.template start< Rule >( in );
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::start( in, st... );
            }
         }

         template< typename ParseInput, typename... States >
         static void success( const ParseInput& in, State& state, States&&... st )
         {
            state.template success< Rule >( in );
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::success( in, st... );
            }
         }

         template< typename ParseInput, typename... States >
         static void failure( const ParseInput& in, State& state, States&&... st )
         {
            state.template failure< Rule >( in );
            if constexpr( Control< Rule >::enable ) {
               Control< Rule >::failure( in, st... );
            }
         }

         template< typename ParseInput, typename... States >
         [[noreturn]] static void raise( const ParseInput& in, State& state, States&&... st )
         {
            state.template raise< Rule >( in );
            Control< Rule >::raise( in, st... );
         }

         template< template< typename... > class Action, typename Iterator, typename ParseInput, typename... States >
         static auto apply( const Iterator& begin, const ParseInput& in, State& state, States&&... st )
            -> decltype( Control< Rule >::template apply< Action >( begin, in, st... ) )
         {
            state.template apply< Rule >( in );
            return Control< Rule >::template apply< Action >( begin, in, st... );
         }

         template< template< typename... > class Action, typename ParseInput, typename... States >
         static auto apply0( const ParseInput& in, State& state, States&&... st )
            -> decltype( Control< Rule >::template apply0< Action >( in, st... ) )
         {
            state.template apply0< Rule >( in );
            return Control< Rule >::template apply0< Action >( in, st... );
         }

         // TODO: unwind should be a real callback...
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
            if constexpr( !State::template enable< Rule > ) {
               return Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
            }
            else {
               try {
                  return Control< Rule >::template match< A, M, Action, Control2 >( in, st... );
               }
               catch( ... ) {
                  auto& state = std::get< sizeof...( st ) - 1 >( std::tie( st... ) );
                  state.template unwind< Rule >( in );
                  throw;
               }
            }
         }
      };

      template< typename Rule >
      using type = rotate_states_right< control< Rule > >;
   };

   template< bool HideInternal = false, std::size_t Indent = 2, std::size_t InitialIndent = 8 >
   struct tracer_traits
   {
      template< typename Rule >
      static constexpr bool enable = ( HideInternal ? normal< Rule >::enable : true );

      static constexpr std::size_t initial_indent = InitialIndent;
      static constexpr std::size_t indent = Indent;
   };

   using standard_tracer_traits = tracer_traits< true >;
   using complete_tracer_traits = tracer_traits< false >;

   template< typename TracerTraits >
   struct tracer
      : TracerTraits
   {
      std::size_t m_count = 0;
      std::vector< std::size_t > m_stack;
      position m_position;

      template< typename ParseInput >
      explicit tracer( const ParseInput& in )
         : m_position( in.position() )
      {
         // TODO: Safe/restore state of std::cerr
         std::cerr << std::left;
         print_position();
      }

      [[nodiscard]] std::size_t indent() const
      {
         return TracerTraits::initial_indent + TracerTraits::indent * m_stack.size();
      }

      void print_position() const
      {
         std::cerr << std::setw( indent() ) << ' ' << "\033[1;34mposition\033[m " << m_position << '\n';
      }

      void update( const position& p )
      {
         if( m_position != p ) {
            m_position = p;
            print_position();
         }
      }

      template< typename Rule, typename ParseInput >
      void start( const ParseInput& /*unused*/ )
      {
         std::cerr << '#' << std::setw( indent() - 1 ) << ++m_count << "\033[36m" << demangle< Rule >() << "\033[m\n";
         m_stack.push_back( m_count );
      }

      template< typename Rule, typename ParseInput >
      void success( const ParseInput& in )
      {
         const auto prev = m_stack.back();
         m_stack.pop_back();
         std::cerr << std::setw( indent() ) << ' ' << "\033[32msuccess\033[m";
         if( m_count != prev ) {
            std::cerr << " #" << prev << ' ' << "\033[37m" << demangle< Rule >() << "\033[m";
         }
         std::cerr << '\n';
         update( in.position() );
      }

      template< typename Rule, typename ParseInput >
      void failure( const ParseInput& in )
      {
         const auto prev = m_stack.back();
         m_stack.pop_back();
         std::cerr << std::setw( indent() ) << ' ' << "\033[31mfailure\033[m";
         if( m_count != prev ) {
            std::cerr << " #" << prev << ' ' << "\033[37m" << demangle< Rule >() << "\033[m";
         }
         std::cerr << '\n';
         update( in.position() );
      }

      template< typename Rule, typename ParseInput >
      void raise( const ParseInput& /*unused*/ )
      {
         std::cerr << std::setw( indent() ) << ' ' << "\033[1;31mraise\033[m \033[36m" << demangle< Rule >() << "\033[m\n";
      }

      template< typename Rule, typename ParseInput >
      void unwind( const ParseInput& in )
      {
         const auto prev = m_stack.back();
         m_stack.pop_back();
         std::cerr << std::setw( indent() ) << ' ' << "\033[31munwind\033[m";
         if( m_count != prev ) {
            std::cerr << " #" << prev << ' ' << "\033[37m" << demangle< Rule >() << "\033[m";
         }
         std::cerr << '\n';
         update( in.position() );
      }

      template< typename Rule, typename ParseInput >
      void apply( const ParseInput& /*unused*/ )
      {
         std::cerr << std::setw( static_cast< int >( indent() - 2 ) ) << ' ' << "\033[1;36mapply\033[m\n";
      }

      template< typename Rule, typename ParseInput >
      void apply0( const ParseInput& /*unused*/ )
      {
         std::cerr << std::setw( static_cast< int >( indent() - 2 ) ) << ' ' << "\033[1;36mapply0\033[m\n";
      }

      template< typename Rule,
                template< typename... > class Action = nothing,
                template< typename... > class Control = normal,
                typename ParseInput,
                typename... States >
      bool parse( ParseInput&& in, States&&... st )
      {
         return TAO_PEGTL_NAMESPACE::parse< Rule, Action, state_control< Control, tracer >::template type >( in, st..., *this );
      }
   };

   using standard_tracer = tracer< standard_tracer_traits >;
   using complete_tracer = tracer< complete_tracer_traits >;

   template< typename Tracer >
   struct trace
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
         if constexpr( sizeof...( st ) == 0 ) {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, state_control< Control, Tracer >::template type >( in, st..., Tracer( in ) );
         }
         else if constexpr( !std::is_same_v< std::tuple_element_t< sizeof...( st ) - 1, std::tuple< States... > >, Tracer& > ) {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, state_control< Control, Tracer >::template type >( in, st..., Tracer( in ) );
         }
         else {
            return TAO_PEGTL_NAMESPACE::match< Rule, A, M, Action, Control >( in, st... );
         }
      }
   };

   using trace_standard = trace< standard_tracer >;
   using trace_complete = trace< complete_tracer >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
