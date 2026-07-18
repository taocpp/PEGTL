// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ACTION_REPEAT_TRAITS_HPP
#define TAO_PEGTL_ACTION_REPEAT_TRAITS_HPP

#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../parse_error.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   struct repeat_traits;

   template< typename T, typename Allocator >
   struct repeat_traits< std::vector< T, Allocator > >
   {
      using element_t = T;

      template< typename ParseInput >
      static void add( const ParseInput& /*unused*/, std::vector< T, Allocator >& out, T&& value )
      {
         out.emplace_back( std::move( value ) );
      }
   };

   template< typename T, typename Allocator >
   struct repeat_traits< std::list< T, Allocator > >
   {
      using element_t = T;

      template< typename ParseInput >
      static void add( const ParseInput& /*unused*/, std::list< T, Allocator >& out, T&& value )
      {
         out.emplace_back( std::move( value ) );
      }
   };

   template< typename T, typename Compare, typename Allocator >
   struct repeat_traits< std::set< T, Compare, Allocator > >
   {
      using element_t = T;

      template< typename ParseInput >
      static void add( const ParseInput& in, std::set< T, Compare, Allocator >& out, T&& value )
      {
         if( !out.emplace( std::move( value ) ).second ) {
            throw_parse_error( "duplicate set value", in );
         }
      }
   };

   template< typename T, typename Compare, typename Allocator >
   struct repeat_traits< std::multiset< T, Compare, Allocator > >
   {
      using element_t = T;

      template< typename ParseInput >
      static void add( const ParseInput& /*unused*/, std::multiset< T, Compare, Allocator >& out, T&& value )
      {
         out.emplace( std::move( value ) );
      }
   };

   template< typename K, typename V, typename Compare, typename Allocator >
   struct repeat_traits< std::map< K, V, Compare, Allocator > >
   {
      using element_t = std::pair< K, V >;

      template< typename ParseInput >
      static void add( const ParseInput& in, std::map< K, V, Compare, Allocator >& out, element_t&& value )
      {
         if( !out.emplace( std::move( value ) ).second ) {
            throw_parse_error( "duplicate map key", in );
         }
      }
   };

   template< typename K, typename V, typename Compare, typename Allocator >
   struct repeat_traits< std::multimap< K, V, Compare, Allocator > >
   {
      using element_t = std::pair< K, V >;

      template< typename ParseInput >
      static void add( const ParseInput& /*unused*/, std::multimap< K, V, Compare, Allocator >& out, element_t&& value )
      {
         out.emplace( std::move( value ) );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
