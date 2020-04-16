// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ANALYZE_TYPE_HPP
#define TAO_PEGTL_CONTRIB_ANALYZE_TYPE_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   enum class analyze_type : std::uint8_t
   {
      any,  // Consumption-on-success is always true; assumes bounded repetition of conjunction of sub-rules.
      opt,  // Consumption-on-success not necessarily true; assumes bounded repetition of conjunction of sub-rules.
      seq,  // Consumption-on-success depends on consumption of (non-zero bounded repetition of) conjunction of sub-rules.
      sor   // Consumption-on-success depends on consumption of (non-zero bounded repetition of) disjunction of sub-rules.
   };

   template< typename... Rules >
   struct analyze_any_traits
   {
      static constexpr analyze_type type_v = analyze_type::any;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_opt_traits
   {
      static constexpr analyze_type type_v = analyze_type::opt;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_seq_traits
   {
      static constexpr analyze_type type_v = analyze_type::seq;
      using subs_t = type_list< Rules... >;
   };

   template< typename... Rules >
   struct analyze_sor_traits
   {
      static constexpr analyze_type type_v = analyze_type::sor;
      using subs_t = type_list< Rules... >;
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
