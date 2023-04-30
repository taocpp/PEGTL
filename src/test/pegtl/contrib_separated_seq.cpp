// Copyright (c) 2021-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/separated_seq.hpp>

#include <type_traits>

namespace tao
{
   using namespace TAO_PEGTL_NAMESPACE;
   // clang-format off
   struct A : one<'0'> {};
   struct B {};
   struct C {};
   struct D {};
   
   struct S {};
   // clang-format on

   static_assert( std::is_base_of< internal::seq<>, separated_seq< S > >::value );
   static_assert( std::is_base_of< internal::seq< A >, separated_seq< S, A > >::value );
   static_assert( std::is_base_of< internal::seq< A, S, B >, separated_seq< S, A, B > >::value );
   static_assert( std::is_base_of< internal::seq< A, S, B, S, C, S, D >, separated_seq< S, A, B, C, D > >::value );
}  // namespace tao