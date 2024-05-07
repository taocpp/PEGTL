// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_INTERNAL_HPP
#define TAO_PEGTL_DEBUG_INTERNAL_HPP

#include <set>
#include <utility>
#include <vector>

#include "../config.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename... Cs >
   class [[nodiscard]] set_stack_guard
   {
   public:
      template< typename... Ts >
      set_stack_guard( std::set< Cs... >& set, Ts&&... ts )
         : m_i( set.emplace( std::forward< Ts >( ts )... ) ),
           m_s( set )
      {}

      set_stack_guard( set_stack_guard&& ) = delete;
      set_stack_guard( const set_stack_guard& ) = delete;

      void operator=( set_stack_guard&& ) = delete;
      void operator=( const set_stack_guard& ) = delete;

      ~set_stack_guard()
      {
         if( m_i.second ) {
            m_s.erase( m_i.first );
         }
      }

      [[nodiscard]] explicit operator bool() const noexcept
      {
         return m_i.second;
      }

   private:
      const std::pair< typename std::set< Cs... >::iterator, bool > m_i;
      std::set< Cs... >& m_s;
   };

   template< typename... Cs >
   set_stack_guard( std::set< Cs... >&, const typename std::set< Cs... >::value_type& ) -> set_stack_guard< Cs... >;

   template< typename... Cs >
   class [[nodiscard]] vector_stack_guard
   {
   public:
      template< typename... Ts >
      vector_stack_guard( std::vector< Cs... >& vector, Ts&&... ts )
         : m_s( vector )
      {
         m_s.emplace_back( std::forward< Ts >( ts )... );
      }

      vector_stack_guard( vector_stack_guard&& ) = delete;
      vector_stack_guard( const vector_stack_guard& ) = delete;

      void operator=( vector_stack_guard&& ) = delete;
      void operator=( const vector_stack_guard& ) = delete;

      ~vector_stack_guard()
      {
         m_s.pop_back();
      }

   private:
      std::vector< Cs... >& m_s;
   };

   template< typename... Cs >
   vector_stack_guard( std::vector< Cs... >&, const typename std::vector< Cs... >::value_type& ) -> vector_stack_guard< Cs... >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
