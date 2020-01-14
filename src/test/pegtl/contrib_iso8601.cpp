// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include "test.hpp"
#include "verify_analyze.hpp"
#include "verify_fail.hpp"

#define WIP 1

#ifdef WIP
#include "result_type.hpp"
#include "verify_impl.hpp"
#else
#include "verify_rule.hpp"
#endif

#include <tao/pegtl/contrib/iso8601.hpp>
#include <tao/pegtl/contrib/tracer.hpp>
#include <tao/pegtl/parse.hpp>

namespace TAO_PEGTL_NAMESPACE
{
#ifdef WIP
   template< typename Rule, typename Eol = eol::lf_crlf >
   void verify_rule( const std::size_t line, const char* file, const std::string& data, const result_type expected, const std::size_t remain )
   {
      memory_input< tracking_mode::eager, Eol > in( data.data(), data.data() + data.size(), file, 0, line, 0 );
      bool rv = verify_impl_one< Rule, nothing >( line, file, data, in, expected, remain );
      if( !rv ) {
         std::cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
         parse< Rule, nothing, tracer >( in );
         std::terminate();
      }
   }
#endif

   using GRAMMAR = must< iso8601::iso8601, eof >;

   void unit_test()
   {
      verify_analyze< GRAMMAR >( __LINE__, __FILE__, true, false );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1981-04-06T04:35:00", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1981-04-06T04:35:00Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1981-04-06T04:35:00+01:00", result_type::success, 0 );

      // dates ////////////////////////////////////////////////////////////////
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19", result_type::success, 0 );
      // // expanded
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+0019850412", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985-04-12", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985-04", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+0019", result_type::success, 0 );

      // // ordinal dates ////////////////////////////////////////////////////////
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985102", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-102", result_type::success, 0 );
      // // expanded
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985102", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985-102", result_type::success, 0 );

      // week dates ////////////////////////////////////////////////////////
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985W155", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-W15-5", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985W15", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-W15", result_type::success, 0 );
      // // expanded
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985W155", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985-W15-5", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985W15", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "+001985-W15", result_type::success, 0 );

      // time of day //////////////////////////////////////////////////////////
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "232050", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23:20:50", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "232050,5", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23:20:50,5", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "2320,8", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23:20,8", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23,3", result_type::success, 0 );

      // midnight - chage with newer iso - not relevant for parsing
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "000000", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "00:00:00", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "240000", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "24:00:00", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "232030Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "2320Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23:20:30Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "23:20Z", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "152746+0100", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "152746-0500", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "152746+01", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "152746-05", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "15:27:46+01:00", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "15:27:46-05:00", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "15:27:46+01", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "15:27:46-05", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T101530", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T101530Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T101530+0400", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T101530+04", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T10:15:30", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T10:15:30Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T10:15:30+04:00", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T10:15:30+04", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T1015", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T10:15", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985102T1015Z", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-102T10:15Z", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985W155T1015+0400", result_type::success, 0 );
      verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-W15-5T10:15+04", result_type::success, 0 );

      // // time intervals ///////////////////////////////////////////////////////
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T232050/19850625T103000", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T23:20:50/1985-06-25T10:30:00", result_type::success, 0 );

      // // durations
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P2Y10M15DT10H30M20S", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P6W", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P00021015T10302", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P0002-10-15T10:30:20", result_type::success, 0 );

      // // durations with start or end
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T232050/P1Y2M15DT12H30M0S", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "19850412T232050/P00010215T12300", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T23:20:50/P1Y2M15DT12H30M0S", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "1985-04-12T23:20:50/P0001-02-15T12:30:00", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P1Y2M15DT12H30M0S/19850412T23205", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P00010215T123000/19850412T23205", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P1Y2M15DT12H30M0S/1985-04-12T23:20:50", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "P0001-02-15T12:30:00/1985-04-12T23:20:5", result_type::success, 0 );

      // // recurring
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/19850412T232050/19850625T103000", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/P2Y10M15DT10H30M20", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/19850412T232050/P1Y2M15DT12H30M0", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/P1Y2M15DT12H30M0S/19850412T23205", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/l985-04-12T23:20:50/1985-06-25T10:30:00", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/1985-04-12T23:20:50/P1Y2M15DT12H30M0", result_type::success, 0 );
      // verify_rule< GRAMMAR >( __LINE__, __FILE__, "R12/P1Y2M15DT12H30M0S/1985-04-12T23:20:5", result_type::success, 0 );

      verify_rule< GRAMMAR >( __LINE__, __FILE__, "", result_type::global_failure, 0 );

      verify_fail< GRAMMAR >( __LINE__, __FILE__, "" );
   }

}  // namespace TAO_PEGTL_NAMESPACE

#include "main.hpp"
