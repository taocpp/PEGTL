// Copyright (c) 2016-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_STREAM_PREFETCH_HPP
#define TAO_PEGTL_STREAM_PREFETCH_HPP

#include <cstddef>

#include "../config.hpp"
#include "../type_list.hpp"

#include "../internal/success.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< std::size_t Num >
   struct prefetch
   {
      using rule_t = prefetch;
      using subs_t = empty_list;

      template< typename ParseInput >
      [[nodiscard]] static bool match( ParseInput& in ) noexcept( noexcept( in.size( Num ) ) )
      {
         (void)in.size( Num );
         return true;
      }
   };

   template<>
   struct prefetch< 0 >
      : internal::success
   {};

}  // namespace TAO_PEGTL_NAMESPACE

#endif
