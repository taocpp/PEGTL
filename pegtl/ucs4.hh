// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_UCS4_HH
#define PEGTL_UCS4_HH

#include "internal/rules.hh"
#include "internal/peek_ucs4.hh"
#include "internal/result_on_found.hh"

namespace pegtl
{
   namespace ucs4
   {
      struct any : internal::any< internal::peek_ucs4 > {};
      template< char32_t ... Cs > struct not_one : internal::one< internal::result_on_found::FAILURE, internal::peek_ucs4, Cs ... > {};
      template< char32_t Lo, char32_t Hi > struct not_range : internal::range< internal::result_on_found::FAILURE, internal::peek_ucs4, Lo, Hi > {};
      template< char32_t ... Cs > struct one : internal::one< internal::result_on_found::SUCCESS, internal::peek_ucs4, Cs ... > {};
      template< char32_t Lo, char32_t Hi > struct range : internal::range< internal::result_on_found::SUCCESS, internal::peek_ucs4, Lo, Hi > {};
      template< char32_t ... Cs > struct ranges : internal::ranges< internal::peek_ucs4, Cs ... > {};

   } // ucs4

} // pegtl

#endif
