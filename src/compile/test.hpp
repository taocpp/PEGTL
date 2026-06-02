// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_COMPILE_TEST_HPP
#define TAO_PEGTL_SRC_COMPILE_TEST_HPP

#if !defined( TAO_PEGTL_COMPILE_REJECT ) || !defined( TAO_PEGTL_COMPILE_ACCEPT )
#error "Compile tests require TAO_PEGTL_COMPILE_REJECT and TAO_PEGTL_COMPILE_ACCEPT!"
#endif

#if ( TAO_PEGTL_COMPILE_REJECT + TAO_PEGTL_COMPILE_ACCEPT ) != 1
#error "Exactly one compile test mode must be enabled!"
#endif

#endif
