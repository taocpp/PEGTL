// Copyright (c) 2014-2021 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_IFMT_HPP
#define TAO_PEGTL_SRC_TEST_PEGTL_VERIFY_IFMT_HPP

#include <tao/pegtl.hpp>

#include "verify_meta.hpp"
#include "verify_rule.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< template< typename, typename, typename > class S >
   void verify_ifmt( const result_type failure = result_type::local_failure )
   {
      verify_analyze< S< eof, eof, eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< S< eof, eof, any > >( __LINE__, __FILE__, false, false );
      verify_analyze< S< eof, any, eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< S< eof, any, any > >( __LINE__, __FILE__, true, false );
      verify_analyze< S< any, eof, eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< S< any, eof, any > >( __LINE__, __FILE__, true, false );
      verify_analyze< S< any, any, eof > >( __LINE__, __FILE__, false, false );
      verify_analyze< S< any, any, any > >( __LINE__, __FILE__, true, false );

      verify_rule< S< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "", failure, 0 );
      verify_rule< S< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "b", failure, 1 );
      verify_rule< S< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "c", result_type::success, 0 );
      verify_rule< S< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ab", result_type::success, 0 );
      verify_rule< S< one< 'a' >, one< 'b' >, one< 'c' > > >( __LINE__, __FILE__, "ac", failure, 2 );

#if defined( __cpp_exceptions )
      verify_rule< must< S< one< 'a' >, one< 'b' >, one< 'c' > > > >( __LINE__, __FILE__, "", result_type::global_failure, 0 );
      verify_rule< must< S< one< 'a' >, one< 'b' >, one< 'c' > > > >( __LINE__, __FILE__, "a", result_type::global_failure, 0 );
      verify_rule< must< S< one< 'a' >, one< 'b' >, one< 'c' > > > >( __LINE__, __FILE__, "ac", result_type::global_failure, 1 );
      verify_rule< must< S< one< 'a' >, one< 'b' >, one< 'c' > > > >( __LINE__, __FILE__, "b", result_type::global_failure, 1 );

      verify_rule< must< S< one< 'a' >, one< 'b' >, seq< one< 'c' >, one< 'd' > > > > >( __LINE__, __FILE__, "c", result_type::global_failure, 0 );
      verify_rule< must< S< one< 'a' >, one< 'b' >, seq< one< 'c' >, one< 'd' > > > > >( __LINE__, __FILE__, "cc", result_type::global_failure, 1 );
#endif
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
