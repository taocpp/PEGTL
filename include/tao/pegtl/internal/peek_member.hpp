// Copyright (c) 2022-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PEEK_MEMBER_HPP
#define TAO_PEGTL_INTERNAL_PEEK_MEMBER_HPP

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "../config.hpp"

#include "data_and_size.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T, T, typename = void >
   struct peek_member_impl;

   // For data members of type 'T' or 'const T'.

   template< typename C, typename T, T C::*P >
   struct peek_member_impl< T C::*, P, std::enable_if_t< std::is_member_object_pointer_v< T C::* > > >
   {
      using data_t = std::decay_t< T >;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? &( in.current( offset )->*P ) : nullptr );
      }
   };

   // For data members of type 'T*' or 'const T*' -- will this case ever be used?

   template< typename C, typename T, T* C::*P >
   struct peek_member_impl< T* C::*, P, std::enable_if_t< std::is_member_object_pointer_v< T* C::* > > >
   {
      using data_t = std::decay_t< T >;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? in.current( offset )->*P : nullptr );
      }
   };

   // For data members of type 'T* const' or 'const T* const' -- will this case ever be used?

   template< typename C, typename T, T* const C::*const P >
   struct peek_member_impl< T* const C::*, P, std::enable_if_t< std::is_member_object_pointer_v< T* const C::* > > >
   {
      using data_t = std::decay_t< T >;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? in.current( offset )->*P : nullptr );
      }
   };

   // For global getter functions that return a T.

   template< bool N, typename C, typename T, T ( *P )( const C& ) noexcept( N ) >
   struct peek_member_impl< T ( * )( const C& ) noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, std::uint8_t >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         if( in.size( offset + 1 ) >= ( offset + 1 ) ) {
            return pair_t( P( *in.current( offset ) ), 1 );
         }
         return pair_t();
      }
   };

   // For global getter functions that return a const T&.

   template< bool N, typename C, typename T, const T& ( *P )( const C& ) noexcept( N ) >
   struct peek_member_impl< const T& ( * )( const C& ) noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? &P( *in.current( offset ) ) : nullptr );
      }
   };

   // For global getter functions that return a const T*.

   template< bool N, typename C, typename T, const T* ( *P )( const C& ) noexcept( N ) >
   struct peek_member_impl< const T* ( * )( const C& ) noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? P( *in.current( offset ) ) : nullptr );
      }
   };

   // For member getter functions that return a T.

   template< bool N, typename C, typename T, T ( C::*P )() const noexcept( N ) >
   struct peek_member_impl< T ( C::* )() const noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, std::uint8_t >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         if( in.size( offset + 1 ) >= ( offset + 1 ) ) {
            return pair_t( ( in.current( offset )->*P )(), 1 );
         }
         return pair_t();
      }
   };

   // For member getter functions that return a const T&.

   template< bool N, typename C, typename T, const T& ( C::*P )() const noexcept( N ) >
   struct peek_member_impl< const T& ( C::* )() const noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? &( ( in.current( offset )->*P )() ) : nullptr );
      }
   };

   // For member getter functions that return a const T*.

   template< bool N, typename C, typename T, const T* ( C::*P )() const noexcept( N ) >
   struct peek_member_impl< const T* ( C::* )() const noexcept( N ), P >
   {
      using data_t = T;
      using pair_t = data_and_size< data_t, void >;

      template< typename ParseInput >
      [[nodiscard]] static pair_t peek( ParseInput& in, const std::size_t offset = 0 ) noexcept( N && noexcept( in.size( 1 ) ) )
      {
         return data_and_size( ( in.size( offset + 1 ) >= ( offset + 1 ) ) ? ( in.current( offset )->*P )() : nullptr );
      }
   };

   template< auto M >
   struct peek_member
      : peek_member_impl< decltype( M ), M >
   {
      template< typename ParseInput >
      [[nodiscard]] static constexpr bool bulk() noexcept
      {
         return true;
      }

      template< typename ParseInput >
      [[nodiscard]] static constexpr std::size_t size() noexcept
      {
         return 1;
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
