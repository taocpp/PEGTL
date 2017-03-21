// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef PEGTL_INTERNAL_INTEGER_SEQUENCE_HH
#define PEGTL_INTERNAL_INTEGER_SEQUENCE_HH

#include <cstddef>
#include <utility>
#include <type_traits>

namespace PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename T, T... Ns >
      struct integer_sequence
      {
         using value_type = T;

         static constexpr std::size_t size() noexcept
         {
            return sizeof...( Ns );
         }
      };

      template< std::size_t ... Ns >
      using index_sequence = integer_sequence< std::size_t, Ns ... >;

      template< typename, std::size_t, bool >
      struct double_up;

      template< typename T, T ... Ns, std::size_t N >
      struct double_up< integer_sequence< T, Ns ... >, N, false >
      {
         using type = integer_sequence< T, Ns ..., ( N + Ns ) ... >;
      };

      template< typename T, T ... Ns, std::size_t N >
      struct double_up< integer_sequence< T, Ns ... >, N, true >
      {
         using type = integer_sequence< T, Ns ..., ( N + Ns ) ..., 2 * N >;
      };

      template< typename T, T N, typename = void >
      struct generate_sequence;

      template< typename T, T N >
      using generate_sequence_t = typename generate_sequence< T, N >::type;

      template< typename T, T N, typename >
      struct generate_sequence
           : double_up< generate_sequence_t< T, N / 2 >, N / 2, N % 2 == 1 >
      {};

      template< typename T, T N >
      struct generate_sequence< T, N, typename std::enable_if< ( N == 0 ) >::type >
      {
         using type = integer_sequence< T >;
      };

      template< typename T, T N >
      struct generate_sequence< T, N, typename std::enable_if< ( N == 1 ) >::type >
      {
         using type = integer_sequence< T, 0 >;
      };

      template< typename T, T N >
      using make_integer_sequence = generate_sequence_t< T, N >;

      template< std::size_t N >
      using make_index_sequence = make_integer_sequence< std::size_t, N >;

      template< typename ... Ts >
      using index_sequence_for = make_index_sequence< sizeof...( Ts ) >;

   } // namespace internal

} // namespace PEGTL_NAMESPACE

#endif
