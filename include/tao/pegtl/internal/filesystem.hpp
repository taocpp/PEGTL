// Copyright (c) 2020-2022 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_INTERNAL_FILESYSTEM_HPP
#define TAO_PEGTL_INTERNAL_FILESYSTEM_HPP

#if defined( TAO_PEGTL_BOOST_FILESYSTEM )

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

namespace tao::pegtl::internal
{
   namespace filesystem = ::boost::filesystem;

   using error_code = ::boost::system::error_code;

   inline const auto& system_category() noexcept
   {
      return ::boost::system::system_category();
   }

}  // namespace tao::pegtl::internal

#elif defined( TAO_PEGTL_STD_EXPERIMENTAL_FILESYSTEM )

#include <experimental/filesystem>

namespace tao::pegtl::internal
{
   namespace filesystem = ::std::experimental::filesystem;

   using error_code = ::std::error_code;

   inline const auto& system_category() noexcept
   {
      return ::std::system_category();
   }

}  // namespace tao::pegtl::internal

#else

#include <filesystem>

namespace tao::pegtl::internal
{
   namespace filesystem = ::std::filesystem;

   using error_code = ::std::error_code;

   inline const auto& system_category() noexcept
   {
      return ::std::system_category();
   }

}  // namespace tao::pegtl::internal

#endif

#endif
