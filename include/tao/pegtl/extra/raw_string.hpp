// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_RAW_STRING_HPP
#define TAO_PEGTL_EXTRA_RAW_STRING_HPP

#include "../config.hpp"

#include "../debug/analyze_traits.hpp"
#include "../internal/until.hpp"

#include "internal/raw_string.hpp"
#include "internal/remove_first_state.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< char Open, char Marker, char Close, typename... Contents >
   struct raw_string
      : internal::raw_string< Open, Marker, Close, internal::until< internal::at_raw_string_close< Marker, Close >, internal::remove_first_state< Contents... > > >
   {
      // struct content;
   };

   template< char Open, char Marker, char Close >
   struct raw_string< Open, Marker, Close >
      : internal::raw_string< Open, Marker, Close, internal::until< internal::at_raw_string_close< Marker, Close > > >
   {
      // struct content;
   };

   template< typename Name, char Open, char Marker >
   struct analyze_traits< Name, internal::raw_string_open< Open, Marker > >
      : analyze_any_traits<>
   {};

   template< typename Name, char Marker, char Close >
   struct analyze_traits< Name, internal::at_raw_string_close< Marker, Close > >
      : analyze_opt_traits<>
   {};

   template< typename Name, char Open, char Marker, char Close, typename Content >
   struct analyze_traits< Name, internal::raw_string< Open, Marker, Close, Content > >
      : analyze_seq_traits< internal::raw_string_open< Open, Marker >, Content >
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
