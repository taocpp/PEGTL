// Copyright (c) 2017-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_TRACKING_MODE_HPP
#define TAO_PEGTL_TRACKING_MODE_HPP

namespace tao::pegtl
{
   enum class tracking_mode : bool
   {
      eager,
      lazy
   };

}  // namespace tao::pegtl

#endif
