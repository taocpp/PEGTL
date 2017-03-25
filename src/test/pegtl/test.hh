// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_TEST_TEST_HH
#define TAOCPP_PEGTL_INCLUDE_TEST_TEST_HH

#include <cstddef>
#include <cassert>
#include <iostream>

#include <tao/pegtl.hh>

namespace tao
{
   namespace pegtl
   {
      std::size_t failed = 0;
      std::vector< std::pair< std::string, std::string > > applied;

   } // namespace pegtl

} // namespace tao

#include "test_action.hh"
#include "test_assert.hh"
#include "test_failed.hh"

#include "verify_rule.hh"
#include "verify_char.hh"
#include "verify_fail.hh"

#include "verify_analyze.hh"

#endif
