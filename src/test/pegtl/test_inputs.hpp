// Copyright (c) 2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_TEST_INPUTS_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_TEST_INPUTS_HPP

#include <tao/pegtl/eol.hpp>
#include <tao/pegtl/internal/inputs.hpp>

namespace TAO_PEGTL_NAMESPACE::test
{
   template< typename Eol >
   using lazy_input = internal::input_with_peeks< internal::lazy_input< Eol, internal::input_with_fakes< internal::input_with_start< char > > > >;

   template< typename Eol >
   using text_input = internal::input_with_peeks< internal::text_input< Eol, internal::input_with_fakes< internal::view_input< char > > > >;

   template< typename Eol >
   using file_input = internal::input_with_peeks< internal::input_with_fakes< internal::text_file_input_with_source< Eol > > >;

}  // namespace TAO_PEGTL_NAMESPACE::test

#endif
