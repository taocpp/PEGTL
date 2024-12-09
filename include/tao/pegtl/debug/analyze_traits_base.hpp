// Copyright (c) 2020-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_ANALYZE_TRAITS_BASE_HPP
#define TAO_PEGTL_DEBUG_ANALYZE_TRAITS_BASE_HPP

#include "../config.hpp"
#include "../type_list.hpp"

#include "analyze_traits_enum.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   struct analyze_any_traits
   {
      // Consumption-on-success is always true; assumes bounded repetition of conjunction of sub-rules.
      static constexpr analyze_traits_enum enum_v = analyze_traits_enum::any;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_opt_traits
   {
      // Consumption-on-success not necessarily true; assumes bounded repetition of conjunction of sub-rules.
      static constexpr analyze_traits_enum enum_v = analyze_traits_enum::opt;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_seq_traits
   {
      // Consumption-on-success depends on consumption of (non-zero bounded repetition of) conjunction of sub-rules.
      static constexpr analyze_traits_enum enum_v = analyze_traits_enum::seq;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_sor_traits
   {
      // Consumption-on-success depends on consumption of (non-zero bounded repetition of) disjunction of sub-rules.
      static constexpr analyze_traits_enum enum_v = analyze_traits_enum::sor;
      using subs_t = type_list< Rules... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif  // FCOV_EXCL_FILE
