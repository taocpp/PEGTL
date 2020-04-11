// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ROTATE_STATES_RIGHT_HPP
#define TAO_PEGTL_CONTRIB_ROTATE_STATES_RIGHT_HPP

#include <tuple>
#include <utility>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   // Applies to start(), success(), failure(), raise(), apply(), and apply0():
   // The last state is rotated to the first position when the call is forwarded to Base.
   template< typename Base >
   struct rotate_states_right
      : Base
   {
      template< typename Input, typename Tuple, std::size_t... Is >
      static void start_impl( const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::start( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... ) ) )
      {
         Base::start( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< typename Input, typename... States >
      static void start( const Input& in, States&&... st ) noexcept( noexcept( start_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) ) )
      {
         start_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< typename Input, typename State >
      static void start( const Input& in, State&& st ) noexcept( noexcept( Base::start( in, st ) ) )
      {
         Base::start( in, st );
      }

      template< typename Input, typename Tuple, std::size_t... Is >
      static void success_impl( const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::success( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... ) ) )
      {
         Base::success( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< typename Input, typename... States >
      static void success( const Input& in, States&&... st ) noexcept( noexcept( success_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) ) )
      {
         success_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< typename Input, typename State >
      static void success( const Input& in, State&& st ) noexcept( noexcept( Base::start( in, st ) ) )
      {
         Base::success( in, st );
      }

      template< typename Input, typename Tuple, std::size_t... Is >
      static void failure_impl( const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::failure( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... ) ) )
      {
         Base::failure( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< typename Input, typename... States >
      static void failure( const Input& in, States&&... st ) noexcept( noexcept( failure_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) ) )
      {
         failure_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< typename Input, typename State >
      static void failure( const Input& in, State&& st ) noexcept( noexcept( Base::start( in, st ) ) )
      {
         Base::failure( in, st );
      }

      template< typename Input, typename Tuple, std::size_t... Is >
      [[noreturn]] static void raise_impl( const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ )
      {
         Base::raise( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< typename Input, typename... States >
      [[noreturn]] static void raise( const Input& in, States&&... st )
      {
         raise_impl( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< typename Input, typename State >
      [[noreturn]] static void raise( const Input& in, State&& st ) noexcept( noexcept( Base::start( in, st ) ) )
      {
         Base::raise( in, st );
      }

      template< template< typename... > class Action, typename Iterator, typename Input, typename Tuple, std::size_t... Is >
      static auto apply_impl( const Iterator& begin, const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::template apply< Action >( begin, in, std::get< Is >( t )... ) ) )
         -> decltype( Base::template apply< Action >( begin, in, std::get< Is >( t )... ) )
      {
         return Base::template apply< Action >( begin, in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< template< typename... > class Action, typename Iterator, typename Input, typename... States >
      static auto apply( const Iterator& begin, const Input& in, States&&... st ) noexcept( noexcept( apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) ) )
         -> decltype( apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) )
      {
         return apply_impl< Action >( begin, in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< template< typename... > class Action, typename Iterator, typename Input, typename State >
      static auto apply( const Iterator& begin, const Input& in, State&& st ) noexcept( noexcept( Base::template apply< Action >( begin, in, st ) ) )
         -> decltype( Base::template apply< Action >( begin, in, st ) )
      {
         return Base::template apply< Action >( begin, in, st );
      }

      template< template< typename... > class Action, typename Input, typename Tuple, std::size_t... Is >
      static auto apply0_impl( const Input& in, const Tuple& t, std::index_sequence< Is... > /*unused*/ ) noexcept( noexcept( Base::template apply0< Action >( in, std::get< Is >( t )... ) ) )
         -> decltype( Base::template apply0< Action >( in, std::get< Is >( t )... ) )
      {
         return Base::template apply0< Action >( in, std::get< sizeof...( Is ) >( t ), std::get< Is >( t )... );
      }

      template< template< typename... > class Action, typename Input, typename... States >
      static auto apply0( const Input& in, States&&... st ) noexcept( noexcept( apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) ) )
         -> decltype( apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() ) )
      {
         return apply0_impl< Action >( in, std::tie( st... ), std::make_index_sequence< sizeof...( st ) - 1 >() );
      }

      template< template< typename... > class Action, typename Input, typename State >
      static auto apply0( const Input& in, State&& st ) noexcept( noexcept( Base::template apply0< Action >( in, st ) ) )
         -> decltype( Base::template apply0< Action >( in, st ) )
      {
         return Base::template apply0< Action >( in, st );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
