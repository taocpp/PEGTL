// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_PEGTL_INCLUDE_PEGTL_HPP
#define TAOCPP_PEGTL_INCLUDE_PEGTL_HPP

#include "pegtl/config.hpp"

#include "pegtl/parse.hpp"
#include "pegtl/rules.hpp"
#include "pegtl/ascii.hpp"
#include "pegtl/utf8.hpp"
#include "pegtl/utf16.hpp"
#include "pegtl/utf32.hpp"

// The following files can be included whenever needed; they
// are not included by default because they include <iostream>.
// #include "pegtl/trace.hpp"
// #include "pegtl/analyze.hpp"

#include "pegtl/string_parser.hpp"
#include "pegtl/file_parser.hpp"

#endif
