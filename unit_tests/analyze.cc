// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#include "test.hh"

namespace pegtl
{
   void unit_test()
   {
      {
         struct bar;
         struct foo : seq< digit, bar > {};
         struct bar : plus< foo > {};
         verify_analyze< seq< any, bar > >( __LINE__, __FILE__, true, false );
      } {
         struct bar;
         struct foo : seq< bar, digit > {};
         struct bar : plus< foo > {};
         verify_analyze< seq< bar, any > >( __LINE__, __FILE__, true, true );
      } {
         struct bar;
         struct foo : sor< digit, bar > {};
         struct bar : plus< foo > {};
         verify_analyze< bar >( __LINE__, __FILE__, false, true );
         verify_analyze< foo >( __LINE__, __FILE__, false, true );
         verify_analyze< sor< any, bar > >( __LINE__, __FILE__, false, true );
      } {
         // Excerpt from the Lua 5.3 grammar:
         //  prefixexp ::= var | functioncall | ‘(’ exp ‘)’
         //  functioncall ::=  prefixexp args | prefixexp ‘:’ Name args
         //  var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name
         // Simplified version, equivalent regarding consumption of input:
         struct var;
         struct fun;
         struct exp : sor< var, fun, seq< any, exp, any > > {};
         struct fun : seq< exp, any > {};
         struct var : sor< any, seq< exp, any, exp >, seq< exp, any > > {};
         verify_analyze< exp >( __LINE__, __FILE__, true, true, false );
         verify_analyze< fun >( __LINE__, __FILE__, true, true, false );
         verify_analyze< var >( __LINE__, __FILE__, true, true, false );
      } {
         struct exp : sor< exp, seq< any, exp > > {};
         verify_analyze< exp >( __LINE__, __FILE__, false, true, false );
      } {
         struct tst : until< any > {};
         verify_analyze< tst >( __LINE__, __FILE__, true, false, false );
      } {
         struct tst : until< star< any > > {};
         verify_analyze< tst >( __LINE__, __FILE__, false, false, false );
      } {
         struct tst : until< any, star< any > > {};
         verify_analyze< tst >( __LINE__, __FILE__, true, true, false );
      } {
         struct tst : until< star< any >, star< any > > {};
         verify_analyze< tst >( __LINE__, __FILE__, false, true, false );
      } {
         struct tst : until< any, any > {};
         verify_analyze< tst >( __LINE__, __FILE__, true, false, false );
      } {
         struct tst : until< star< any >, any > {};
         verify_analyze< tst >( __LINE__, __FILE__, false, false, false );
      }
   }

} // pegtl

#include "main.hh"
