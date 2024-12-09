// Copyright (c) 2014-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"

#include <cstdio>

namespace TAO_PEGTL_NAMESPACE
{
   void unit_test()
   {
      const std::filesystem::path path( "src/test/pegtl/data/duseltronik.txt" );
      {
         const internal::read_input in( path );
         const std::string data( in.start(), in.end() );
         TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
      }
      {
         const std::unique_ptr< std::FILE, internal::read_file_close > file( internal::read_file_open( path ) );
         const internal::read_input in( file.get(), path );
         const std::string data( in.start(), in.end() );
         TAO_PEGTL_TEST_ASSERT( data == "duseltronik" );
      }
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
