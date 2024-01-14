// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_ANALYZE_TRAITS_HPP
#define TAO_PEGTL_ANALYZE_TRAITS_HPP

#include "config.hpp"
#include "forward.hpp"
#include "type_list.hpp"

#include "internal/analyze_group.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   struct analyze_any_traits
   {
      // Consumption-on-success is always true; assumes bounded repetition of conjunction of sub-rules.
      static constexpr internal::analyze_group type_v = internal::analyze_group::any;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_opt_traits
   {
      // Consumption-on-success not necessarily true; assumes bounded repetition of conjunction of sub-rules.
      static constexpr internal::analyze_group type_v = internal::analyze_group::opt;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_seq_traits
   {
      // Consumption-on-success depends on consumption of (non-zero bounded repetition of) conjunction of sub-rules.
      static constexpr internal::analyze_group type_v = internal::analyze_group::seq;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_sor_traits
   {
      // Consumption-on-success depends on consumption of (non-zero bounded repetition of) disjunction of sub-rules.
      static constexpr internal::analyze_group type_v = internal::analyze_group::sor;
      using subs_t = type_list< Rules... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#include "internal/analyze_traits.hpp"

#endif
