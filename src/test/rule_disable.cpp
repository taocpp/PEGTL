// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_ctrl.hpp"
#include "verify_meta.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< disable<>, internal::success >();
      verify_meta< disable< eof >, internal::disable< eof >, eof >();
      verify_meta< disable< eof, any >, internal::disable< internal::seq< eof, any > >, internal::seq< eof, any > >();

      verify_seqs< disable >();

      verify_ctrl_enabled< disable< eof > >( __LINE__, __FILE__, "" );
      verify_ctrl_disabled< internal::disable< eof > >( __LINE__, __FILE__, "" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
