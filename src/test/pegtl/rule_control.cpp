// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   using test_control_rule = control< normal, Rules... >;

   void unit_test()
   {
      verify_meta< control< normal >, internal::success >();
      verify_meta< control< normal, eof >, internal::control< normal, eof >, eof >();
      verify_meta< control< normal, eof, any >, internal::control< normal, internal::seq< eof, any > >, internal::seq< eof, any > >();

      verify_seqs< test_control_rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
