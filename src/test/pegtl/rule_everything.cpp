// Copyright (c) 2022-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      verify_meta< everything, internal::everything< std::size_t > >();

      verify_analyze< everything >( __LINE__, __FILE__, false, false );

      verify_rule< everything >( __LINE__, __FILE__, "", result_type::success );
      verify_rule< everything >( __LINE__, __FILE__, "asodfuaofj", result_type::success );
      verify_rule< everything >( __LINE__, __FILE__, "asfhalfj;alsjfdo;asjdfo;asjf;laskjdfl;afwfj2;093vmu;oaivu;v390fm;aosumvos3fefalihgfis", result_type::success );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
