// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_TEXT_COMBINATION_HPP
#define TAO_PEGTL_INTERNAL_TEXT_COMBINATION_HPP

#include <filesystem>

#include "../config.hpp"

#include "input_double_path.hpp"

#include "text_input_with_source.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Eol, typename Input >
   class text_combination
      : public input_double_path< text_input_with_source< Eol, std::filesystem::path, std::filesystem::path, Input > >
   {
   public:
      using input_double_path< text_input_with_source< Eol, std::filesystem::path, std::filesystem::path, Input > >::input_double_path;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
