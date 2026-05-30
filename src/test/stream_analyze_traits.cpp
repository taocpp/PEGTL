// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"

#include <tao/pegtl/stream/analyze_traits.hpp>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_analyze< discard >( __LINE__, __FILE__, false, false );
      verify_analyze< is_stream >( __LINE__, __FILE__, false, false );
      verify_analyze< prefetch< 1 > >( __LINE__, __FILE__, false, false );
      verify_analyze< prefetch< 2 > >( __LINE__, __FILE__, false, false );
      verify_analyze< require< 1 > >( __LINE__, __FILE__, false, false );
      verify_analyze< require< 2 > >( __LINE__, __FILE__, false, false );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
