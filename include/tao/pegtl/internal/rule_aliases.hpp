// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_RULE_ALIASES_HPP
#define TAO_PEGTL_INTERNAL_RULE_ALIASES_HPP

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
   using list_opt = opt< Rule, star< Sep, Rule > >;

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

// FCOV_EXCL_FILE
#endif
