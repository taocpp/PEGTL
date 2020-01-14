// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_ISO8601_HPP
#define TAO_PEGTL_CONTRIB_ISO8601_HPP

#include <cstdint>

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"
#include "../utf8.hpp"

#include "abnf.hpp"
#include "integer.hpp"

namespace TAO_PEGTL_NAMESPACE::iso8601
{
   // DateTime grammar according to ISO-8601. (2004)

   // clang-format off
   struct dash : one< '-' > {};
   struct colon : one< ':' > {};
   struct fsep : one< '.', ',' > {};
   struct sign : one< '+', '-' > {};

   struct T : one< 'T' > {};
   struct Z : one< 'Z' > {};
   struct W : one< 'W' > {};

   template< std::size_t N > using d = rep< N, digit >;

   struct YYYY : d< 4 > {};
   struct YY : d< 2 > {};
   struct year : sor< YYYY, YY > {};

   struct MM : d< 2 > {};
   struct D : d< 1 > {};
   struct DD : d< 2 > {};
   struct DDD : d< 3 > {};
   struct WW : d< 2 > {};

   struct hh : d< 2 > {};
   struct mm : d< 2 > {};
   struct ss : d< 2 > {};
   struct fract : plus< digit > {};

   struct zhh : d< 2 > {};
   struct zmm : d< 2 > {};

   struct extended_date_normal : seq< year, dash, not_at< W >, seq< MM, opt< dash, DD > > > {};
   struct extended_date_ord : seq< YYYY, dash, sor< seq< W, WW, opt< dash, D > >, DDD > > {};

   struct basic_date_normal : seq< year, not_at< W >, opt< seq< MM, opt< DD > > >, not_at< digit > > {};
   struct basic_date_ord : seq< YYYY, sor< seq< W, WW, opt< D > >, DDD >, not_at< digit > > {};

   struct extended_date : sor< extended_date_ord, extended_date_normal > {};
   struct basic_date : sor< basic_date_normal, basic_date_ord > {};

   struct extended_offset : sor< Z, seq< sign, zhh, opt< colon, zmm > > > {};
   struct basic_offset : sor< Z, seq< sign, zhh, opt< zmm > > > {};

   struct extended_time : seq< hh, colon, mm, opt< colon, ss >, opt< fsep, fract >, opt< extended_offset > > {};
   struct basic_time : seq< hh, opt< mm, not_at< W >, opt< ss > >, opt< fsep, fract >, opt< basic_offset >, not_at< digit > > {};

   struct extended_format : sor< extended_time, seq< extended_date, opt< T, extended_time > > > {};
   struct basic_format : sor< basic_time, seq< basic_date, opt< T, basic_time > > > {};

   // It's either basic or extended format, but never mixed.
   struct iso8601 : sor< extended_format, basic_format > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::iso8601

#endif
