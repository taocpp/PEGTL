// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEMANGLE_HPP
#define TAO_PEGTL_DEMANGLE_HPP

#include <string_view>

#include "config.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename T >
   [[nodiscard]] std::string_view demangle() noexcept;

}  // namespace TAO_PEGTL_NAMESPACE

#if defined( __clang__ )

#include "internal/demangle_clang.hpp"

#elif defined( __GNUC__ )

#include "internal/demangle_gcc.hpp"

#elif defined( _MSC_VER )

#include "internal/demangle_msvc.hpp"

#elif defined( __cpp_rtti )

#include "internal/demangle_rtti.hpp"

#else

#error "No demangle implementation available!"

#endif

#endif
