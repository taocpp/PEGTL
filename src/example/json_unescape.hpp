// Copyright (c) 2014-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_EXAMPLE_JSON_UNESCAPE_HPP
#define TAO_PEGTL_SRC_EXAMPLE_JSON_UNESCAPE_HPP

#include <string>

#include <tao/pegtl/action/change_action_and_state.hpp>
#include <tao/pegtl/extra/unescape.hpp>

namespace example
{
   namespace pegtl = TAO_PEGTL_NAMESPACE;

   using json_unescape = pegtl::change_action_and_state< pegtl::unescape, std::string >;

}  // namespace example

#endif
