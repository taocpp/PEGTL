// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PEGTL_HPP
#define TAOCPP_PEGTL_INCLUDE_PEGTL_HPP

#include "pegtl/config.hpp"

#include "pegtl/ascii.hpp"
#include "pegtl/parse.hpp"
#include "pegtl/rules.hpp"
#include "pegtl/utf16.hpp"
#include "pegtl/utf32.hpp"
#include "pegtl/utf8.hpp"

#include "pegtl/parse.hpp"

#include "pegtl/buffer_input.hpp"
#include "pegtl/file_input.hpp"
#include "pegtl/memory_input.hpp"
#include "pegtl/read_input.hpp"
#include "pegtl/stream_input.hpp"
#include "pegtl/string_input.hpp"

// The following files can be included whenever needed; they
// are not included by default because they include a stream header
// like <iostream> or <sstream>.

// #include "pegtl/argv_input.hpp"
// #include "pegtl/analyze.hpp"
// #include "pegtl/tracer.hpp"

#endif
