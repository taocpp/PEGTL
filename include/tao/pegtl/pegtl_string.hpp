// Copyright (c) 2015-2023 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_PEGTL_STRING_HPP
#define TAO_PEGTL_PEGTL_STRING_HPP

#include "ascii.hpp"
#include "config.hpp"
#include "rules.hpp"

#include "internal/pegtl_string.hpp"

#define TAO_PEGTL_STRING( x ) \
   TAO_PEGTL_INTERNAL_STRING( ::TAO_PEGTL_NAMESPACE::ascii::string, x )

#define TAO_PEGTL_ISTRING( x ) \
   TAO_PEGTL_INTERNAL_STRING( ::TAO_PEGTL_NAMESPACE::ascii::istring, x )

#define TAO_PEGTL_KEYWORD( x ) \
   TAO_PEGTL_INTERNAL_STRING( ::TAO_PEGTL_NAMESPACE::ascii::keyword, x )

#define TAO_PEGTL_RAISE_MESSAGE( x ) \
   TAO_PEGTL_INTERNAL_STRING( ::TAO_PEGTL_NAMESPACE::raise_message, x )

#endif
