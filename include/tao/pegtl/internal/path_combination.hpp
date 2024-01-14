// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_PATH_COMBINATION_HPP
#define TAO_PEGTL_INTERNAL_PATH_COMBINATION_HPP

#include <filesystem>

#include "../config.hpp"

#include "input_with_source.hpp"

#include "input_double_path.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename Input >
   class path_combination
      : public input_double_path< input_with_source< std::filesystem::path, std::filesystem::path, Input > >
   {
   public:
      using input_double_path< input_with_source< std::filesystem::path, std::filesystem::path, Input > >::input_double_path;
   };

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
