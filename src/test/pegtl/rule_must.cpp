// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions )
#include <iostream>
int main()
{
   std::cout << "Exception support disabled, skipping test..." << std::endl;
}
#else

#include "test.hpp"
#include "verify_seqs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< must<>, internal::success >();
      verify_meta< must< alpha >, internal::must< alpha >, alpha >();
      verify_meta< must< alpha, digit >, internal::seq< internal::must< alpha >, internal::must< digit > >, internal::must< alpha >, internal::must< digit > >();

      verify_seqs< must >( result_type::global_failure );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"

#endif
