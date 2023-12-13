// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INPUTS_HPP
#define TAO_PEGTL_INPUTS_HPP

#include <string>

#include "config.hpp"

#include "internal/inputs.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   using argv_input = internal::input_with_fakes< internal::input_with_peeks< internal::argv_input > >;  // TODO: Add input_with_start?
   template< typename Container >
   using copy_input = internal::input_with_fakes< internal::input_with_peeks< internal::copy_input< Container > > >;
   using file_input = internal::input_with_fakes< internal::input_with_peeks< internal::file_input > >;
   using read_input = internal::input_with_fakes< internal::input_with_peeks< internal::read_input > >;
   template< typename Data >
   using view_input = internal::input_with_fakes< internal::input_with_peeks< internal::view_input< Data > > >;  // TODO: Add input_with_start?

   using argv_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::argv_input_with_source > >;  // TODO: Add input_with_start?
   template< typename Container >
   using copy_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::input_with_source< std::string, internal::copy_input< Container > > > >;
   using file_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::file_input_with_source > >;
   template< typename Data >
   using view_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::input_with_source< std::string, internal::view_input< Data > > > >;  // TODO: Add input_with_start?

   template< typename Eol >
   using lazy_copy_input = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_input< Eol, internal::copy_input< std::string > > > >;
   template< typename Eol >
   using text_copy_input = internal::input_with_fakes< internal::input_with_peeks< internal::text_input< Eol, internal::copy_input< std::string > > > >;
   template< typename Eol >
   using lazy_file_input = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_input< Eol, internal::file_input > > >;
   template< typename Eol >
   using text_file_input = internal::input_with_fakes< internal::input_with_peeks< internal::text_input< Eol, internal::file_input > > >;
   template< typename Eol >
   using lazy_view_input = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_input< Eol, internal::view_input< char > > > >;  // TODO: Add input_with_start?
   template< typename Eol >
   using text_view_input = internal::input_with_fakes< internal::input_with_peeks< internal::text_input< Eol, internal::view_input< char > > > >;  // TODO: Add input_with_start?

   template< typename Eol >
   using lazy_copy_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_input_with_source< Eol, std::string, internal::copy_input< std::string > > > >;
   template< typename Eol >
   using text_copy_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::text_input_with_source< Eol, std::string, internal::copy_input< std::string > > > >;
   template< typename Eol >
   using lazy_file_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_file_input_with_source< Eol > > >;
   template< typename Eol >
   using text_file_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::text_file_input_with_source< Eol > > >;
   template< typename Eol >
   using lazy_view_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::lazy_input_with_source< Eol, std::string, internal::view_input< char > > > >;  // TODO: Add input_with_start?
   template< typename Eol >
   using text_view_input_with_source = internal::input_with_fakes< internal::input_with_peeks< internal::text_input_with_source< Eol, std::string, internal::view_input< char > > > >;  // TODO: Add input_with_start?

}  // namespace TAO_PEGTL_NAMESPACE

#endif
