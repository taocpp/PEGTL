// Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXAMPLE_OPEN_METRICS_HPP
#define TAO_PEGTL_EXAMPLE_OPEN_METRICS_HPP

#include "../ascii.hpp"
#include "../config.hpp"
#include "../rules.hpp"

#include "../unicode/utf8.hpp"

#include "abnf_core.hpp"

namespace TAO_PEGTL_NAMESPACE::open_metrics
{
   using namespace abnf;

   // PEG-ified version of the Open Metrics text format version 1.0.0.
   // Use with an input that only accepts line-feed '\n' as end-of-line!

   struct BS : one< '\\' > {};
   struct EQ : one< '=' > {};
   struct DOT : one< '.' > {};
   struct HASH : one< '#' > {};
   struct OPEN : one< '{' > {};
   struct SIGN : one< '+', '-' > {};
   struct CLOSE : one< '}' > {};
   struct COMMA : one< ',' > {};

   struct type : string< 'T', 'Y', 'P', 'E' > {};
   struct help : string< 'H', 'E', 'L', 'P' > {};
   struct unit : string< 'U', 'N', 'I', 'T' > {};

   struct trailer : seq< HASH, SP, string< 'E', 'O', 'F' >, eolf, eof > {};

   struct normal_char : utf8::not_one< '\n', '\\', '"' > {};
   struct backslash_char : utf8::not_one< '\n' > {};
   struct escaped_char : if_then_else< BS, backslash_char, normal_char > {};

   struct help_string : star< escaped_char > {};

   struct exemplar_label_value : star< escaped_char > {};
   struct sample_label_value : star< escaped_char > {};

   struct exemplar_label_name : identifier {};
   struct sample_label_name : identifier {};

   struct nan : istring< 'n', 'a', 'n' > {};
   struct infinity : aistring< 3, 'i', 'n', 'f', 'i', 'n', 'i', 't', 'y' > {};

   struct exponent : seq< ione< 'e' >, opt< SIGN >, plus< digit > > {};
   struct real_number_1 : seq< plus< digit >, opt< DOT, star< digit > >, opt< exponent > > {};
   struct real_number_2 : seq< DOT, plus< digit >, opt< exponent > > {};
   struct real_number : seq< opt< SIGN >, sor< real_number_1, real_number_2 > > {};
   struct number : sor< real_number, nan, seq< opt< SIGN >, infinity > > {};

   struct exemplar_number : number {};
   struct sample_number : number {};

   struct exemplar_timestamp : real_number {};
   struct sample_timestamp : real_number {};

   struct counter : string< 'c', 'o', 'u', 'n', 't', 'e', 'r' > {};
   struct gauge : string< 'g', 'a', 'u', 'g', 'e' > {};
   struct histogram : string< 'h', 'i', 's', 't', 'o', 'g', 'r', 'a', 'm' > {};
   struct gaugehistogram : string< 'g', 'a', 'u', 'g', 'e', 'h', 'i', 's', 't', 'o', 'g', 'r', 'a', 'm' > {};
   struct stateset : string< 's', 't', 'a', 't', 'e', 's', 'e', 't' > {};
   struct info : string< 'i', 'n', 'f', 'o' > {};
   struct summary : string< 's', 'u', 'm', 'm', 'a', 'r', 'y' > {};
   struct unknown : string< 'u', 'n', 'k', 'n', 'o', 'w', 'n' > {};
   struct metric_type : sor< counter, gaugehistogram, gauge, histogram, stateset, info, summary, unknown > {};

   struct metric_name_char : sor< alnum, one< ':', '_' > > {};
   struct metric_name_initial_char : sor< alpha, one< ':', '_' > > {};

   struct sample_metric_name : seq< metric_name_initial_char, star< metric_name_char > > {};
   struct descriptor_metric_name : seq< metric_name_initial_char, star< metric_name_char > > {};

   struct unit_string : star< metric_name_char > {};

   struct type_descriptor : seq< type, SP, descriptor_metric_name, SP, metric_type, eol > {};
   struct unit_descriptor : seq< unit, SP, descriptor_metric_name, SP, unit_string, eol > {};
   struct help_descriptor : seq< help, SP, descriptor_metric_name, SP, help_string, eol > {};
   struct metric_descriptor : seq< HASH, SP, sor< type_descriptor, unit_descriptor, help_descriptor > > {};

   struct exemplar_label : seq< exemplar_label_name, EQ, DQUOTE, exemplar_label_value, DQUOTE > {};
   struct sample_label : seq< sample_label_name, EQ, DQUOTE, sample_label_value, DQUOTE > {};

   struct exemplar_labels : seq< OPEN, list_opt< exemplar_label, COMMA >, CLOSE > {};
   struct sample_labels : seq< OPEN, list_opt< sample_label, COMMA >, CLOSE > {};

   struct exemplar : seq< HASH, SP, exemplar_labels, SP, exemplar_number, opt< SP, exemplar_timestamp > > {};
   struct sample : seq< sample_metric_name, opt< sample_labels >, SP, sample_number, opt< SP, sample_timestamp >, opt< SP, exemplar >, eol > {};

   struct content : sor< metric_descriptor, sample > {};
   struct exposition : until< trailer, content > {};

}  // namespace TAO_PEGTL_NAMESPACE::open_metrics

#endif
