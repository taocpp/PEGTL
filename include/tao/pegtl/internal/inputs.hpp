// Copyright (c) 2021-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_INPUTS_HPP
#define TAO_PEGTL_INTERNAL_INPUTS_HPP

#include "argv_input.hpp"
#include "copy_input.hpp"
#include "file_input.hpp"
#include "lazy_input.hpp"
// mmap_input.hpp via file_input.hpp
#include "read_input.hpp"
#include "text_input.hpp"
#include "view_input.hpp"

#include "input_with_fakes.hpp"
#include "input_with_lines.hpp"
#include "input_with_offset.hpp"
#include "input_with_peeks.hpp"
#include "input_with_source.hpp"
#include "input_with_start.hpp"

#include "input_double_path.hpp"

#include "argv_input_with_source.hpp"
#include "text_input_with_source.hpp"

#endif
