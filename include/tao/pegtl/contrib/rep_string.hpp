// Copyright (c) 2019-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_REP_STRING_HPP
#define TAO_PEGTL_CONTRIB_REP_STRING_HPP

#include <cstddef>

#include "../config.hpp"

#include "../internal/ascii_string.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< std::size_t, typename, char... >
      struct make_rep_string;

      template< char... Ss, char... Cs >
      struct make_rep_string< 0, ascii_string< Ss... >, Cs... >
      {
         using type = ascii_string< Ss... >;
      };

      template< std::size_t N, char... Ss, char... Cs >
      struct make_rep_string< N, ascii_string< Ss... >, Cs... >
         : make_rep_string< N - 1, ascii_string< Ss..., Cs... >, Cs... >
      {};

   }  // namespace internal

   inline namespace ascii
   {
      template< std::size_t N, char... Cs >
      struct rep_string
         : internal::make_rep_string< N, internal::ascii_string<>, Cs... >::type
      {};

   }  // namespace ascii

}  // namespace TAO_PEGTL_NAMESPACE

#endif
