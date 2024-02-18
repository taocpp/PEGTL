// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_USING_HPP
#define TAO_PEGTL_INTERNAL_USING_HPP

#include <cstddef>

#include "../config.hpp"

#include "eof.hpp"
#include "not_at.hpp"
#include "one.hpp"
#include "opt.hpp"
#include "rematch.hpp"
#include "rep.hpp"
#include "seq.hpp"
#include "star.hpp"
#include "star_partial.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Rule, typename Pad1 >
   using lpad = seq< star< Pad1 >, Rule >;

   template< typename Rule, typename Pad1, typename Pad2 = Pad1 >
   using pad = seq< star< Pad1 >, Rule, star< Pad2 > >;

   template< typename Rule, typename Pad2 >
   using rpad = seq< Rule, star< Pad2 > >;

   template< typename Rule, typename Sep >
   using list = seq< Rule, star< Sep, Rule > >;

   template< typename Rule, typename Sep >
   using list_tail = seq< Rule, star_partial< Sep, Rule > >;

   template< typename Rule, typename Sep, typename Pad >
   using list_tail_pad = seq< Rule, star_partial< lpad< Sep, Pad >, lpad< Rule, Pad > > >;

   template< typename M, typename S >
   using minus = rematch< M, not_at< S, eof > >;

   template< typename Rule, typename Pad >
   using pad_opt = seq< star< Pad >, opt< Rule, star< Pad > > >;

   template< std::size_t Min, typename Rule, typename... Rules >
   using rep_min = seq< rep< Min, Rule, Rules... >, star< Rule, Rules... > >;

   template< typename Peek, typename Peek::data_t... Cs >
   using seq_one = seq< one< Peek, Cs >... >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#if defined( __cpp_exceptions )

#include "if_must.hpp"
#include "if_then_else.hpp"
#include "must.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Cond, typename Then, typename Else >
   using if_must_else = if_then_else< Cond, must< Then >, must< Else > >;

   template< typename Rule, typename Sep >
   using list_must = seq< Rule, star< Sep, must< Rule > > >;

   template< typename Cond, typename... Rules >
   using star_must = star< if_must< false, Cond, Rules... > >;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
#endif
