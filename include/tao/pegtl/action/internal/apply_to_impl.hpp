// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_IMPL_HPP
#define TAO_PEGTL_ACTION_INTERNAL_APPLY_TO_IMPL_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <tuple>

#include "../../config.hpp"

#include "apply_to_call.hpp"
#include "integer_utility.hpp"
#include "throw_or_terminate.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< auto S, typename Target, typename Object, typename = void >
   struct apply_to_impl;

   template< auto S, typename Target >
   struct apply_to_impl< S, Target, std::string >
      : require_apply
   {
      template< typename ActionInput, typename... States >
      static void apply( ActionInput& in, States&&... st )
      {
         Target& out = std::get< Target& >( std::tie( st... ) );
         apply_to_call< S >::call( out, in.string() );
      }
   };

   template< auto S, typename Target >
   struct apply_to_impl< S, Target, std::string_view >
      : require_apply
   {
      template< typename ActionInput, typename... States >
      static void apply( ActionInput& in, States&&... st )
      {
         Target& out = std::get< Target& >( std::tie( st... ) );
         apply_to_call< S >::call( out, in.string_view() );
      }
   };

   template< auto S, typename Target >
   struct apply_to_impl< S, Target, float >
      : require_apply
   {
      template< typename ActionInput, typename... States >
      static void apply( ActionInput& in, States&&... st )
      {
         std::size_t pos = 0;

         const float result = std::stof( in.string(), &pos );

         if( pos != in.size() ) {
            throw_or_terminate( "trailing garbage after float", in );
         }
         Target& out = std::get< Target& >( std::tie( st... ) );
         apply_to_call< S >::call( out, result );
      }
   };

   template< auto S, typename Target >
   struct apply_to_impl< S, Target, double >
      : require_apply
   {
      template< typename ActionInput, typename... States >
      static void apply( ActionInput& in, States&&... st )
      {
         std::size_t pos = 0;

         const double result = std::stod( in.string(), &pos );

         if( pos != in.size() ) {
            throw_or_terminate( "trailing garbage after double", in );
         }
         Target& out = std::get< Target& >( std::tie( st... ) );
         apply_to_call< S >::call( out, result );
      }
   };

   template< auto S, typename Target, typename Object >
   struct apply_to_impl< S, Target, Object, std::enable_if_t< is_proper_integer_v< Object > > >
      : require_apply
   {
      template< typename ActionInput, typename... States >
      static void apply( const ActionInput& in, States&&... st )
      {
         Target& out = std::get< Target& >( std::tie( st... ) );
         apply_to_call< S >::call( out, convert_integer< Object >( in ) );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
