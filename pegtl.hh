// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_PEGTL_HH
#define PEGTL_PEGTL_HH

#include "pegtl/input.hh"
#include "pegtl/parse.hh"
#include "pegtl/trace.hh"
#include "pegtl/rules.hh"

#include "pegtl/ascii.hh"
#include "pegtl/utf8.hh"
#include "pegtl/ucs4.hh"

// Headers that are not included
// by default because they are not
// always needed and rather large
// and/or include many other files.

// #include "pegtl/trace.hh"
// #include "pegtl/analyze.hh"

#include "pegtl/assertions.hh"

#include "pegtl/data_parser.hh"
#include "pegtl/mmap_parser.hh"
#include "pegtl/read_parser.hh"

#endif
