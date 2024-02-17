// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TEXT_POSITION_WITH_SOURCE_HPP
#define TAO_PEGTL_TEXT_POSITION_WITH_SOURCE_HPP

#include <filesystem>
#include <string>

#include "config.hpp"

#include "position_with_source.hpp"
#include "text_position.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   using text_position_with_path = position_with_source< std::filesystem::path, text_position >;
   using text_position_with_source = position_with_source< std::string, text_position >;

}  // namespace TAO_PEGTL_NAMESPACE

#endif
