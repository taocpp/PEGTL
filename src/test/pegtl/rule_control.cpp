// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename... Rules >
   using test_control_rule = control< normal, Rules... >;

   void unit_test()
   {
      verify_meta< control< normal >, internal::control< normal > >();
      verify_meta< control< normal, eof >, internal::control< normal, eof >, eof >();
      verify_meta< control< normal, eof, any >, internal::control< normal, eof, any >, eof, any >();

      verify_seqs< test_control_rule >();
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
