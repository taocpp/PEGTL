// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_INSERT_RULE_HH
#define PEGTL_ANALYSIS_INSERT_RULE_HH

#include "insert_help.hh"
#include "grammar_info.hh"

namespace pegtl
{
   namespace analysis
   {
      template< typename Name, typename Peek >
      std::string insert_rule( grammar_info & g, const internal::any< Peek > * )
      {
         return g.insert< Name >( rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS ).first->first;
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::at< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
      }

      template< typename Name, bool Want, typename Peek, typename Peek::data_t ... Cs >
      std::string insert_rule( grammar_info & g, const internal::at_one< Want, Peek, Cs ... > * )
      {
         return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
      }

      template< typename Name, bool Want, typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
      std::string insert_rule( grammar_info & g, const internal::at_range< Want, Peek, Lo, Hi > * )
      {
         return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
      }

      template< typename Name, unsigned Num >
      std::string insert_rule( grammar_info & g, const internal::bytes< Num > * )
      {
         if ( Num ) {
            return g.insert< Name >( rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS ).first->first;
         }
         else {
            return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
         }
      }

      template< typename Name >
      std::string insert_rule( grammar_info & g, const internal::eof * )
      {
         return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
      }

      template< typename Name >
      std::string insert_rule( grammar_info & g, const internal::eolf * )
      {
         return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
      }

      template< typename Name, typename Cond, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::if_mode< error_mode::THROW, Cond, Rules ... > * )
      {
         return insert_help< Name, Cond, internal::seq< Rules ... >, internal::trivial< false > >( g, rule_type::RULE_IF_MUST_ELSE );
      }

      template< typename Name, typename Cond, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::if_mode< error_mode::RETURN, Cond, Rules ... > * )
      {
         return insert_help< Name, Cond, internal::seq< Rules ... >, internal::trivial< true > >( g, rule_type::RULE_IF_THEN_ELSE );
      }

      template< typename Name, typename Cond, typename Rule, typename Else >
      std::string insert_rule( grammar_info & g, const internal::if_mode_else< error_mode::THROW, Cond, Rule, Else > * )
      {
         return insert_help< Name, Cond, Rule, Else >( g, rule_type::RULE_IF_MUST_ELSE );
      }

      template< typename Name, typename Cond, typename Rule, typename Else >
      std::string insert_rule( grammar_info & g, const internal::if_mode_else< error_mode::RETURN, Cond, Rule, Else > * )
      {
         return insert_help< Name, Cond, Rule, Else >( g, rule_type::RULE_IF_THEN_ELSE );
      }

      template< typename Name, char ... Cs >
      std::string insert_rule( grammar_info & g, const internal::istring< Cs ... > * )
      {
         if ( sizeof ... ( Cs ) ) {
            return g.insert< Name >( rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS ).first->first;
         }
         else {
            return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
         }
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::not_at< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::opt< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
      }

      template< typename Name, typename Rule, typename Pad1, typename Pad2 >
      std::string insert_rule( grammar_info & g, const internal::pad< Rule, Pad1, Pad2 > * )
      {
         return insert_help< Name, Rule >( g, rule_type::CONJUNCTION );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::plus< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CONJUNCTION );
      }

      template< typename Name, unsigned Num, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::rep< Num, Rules ... > * )
      {
         static_assert( Num, "internal error -- this should have been prevented by internal_t indicating a different type" );
         return insert_help< Name, Rules ... >( g, rule_type::CONJUNCTION );
      }

      template< typename Name, unsigned Min, unsigned Max, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::rep_min_max< Min, Max, Rules ... > * )
      {
         if ( Min ) {
            return insert_help< Name, Rules ... >( g, rule_type::CONJUNCTION );
         }
         else {
            return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
         }
      }

      template< typename Name, unsigned Num, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::rep_opt< Num, Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::seq< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CONJUNCTION );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::sor< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::DISJUNCTION );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::star< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CAN_SUCCEED_WITHOUT_CONSUME );
      }

      template< typename Name, char ... Cs >
      std::string insert_rule( grammar_info & g, const internal::string< Cs ... > * )
      {
         if ( sizeof ... ( Cs ) ) {
            return g.insert< Name >( rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS ).first->first;
         }
         else {
            return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
         }
      }

      template< typename Name >
      std::string insert_rule( grammar_info & g, const internal::trivial< true > * )
      {
         return g.insert< Name >( rule_type::CAN_SUCCEED_WITHOUT_CONSUME ).first->first;
      }

      template< typename Name >
      std::string insert_rule( grammar_info & g, const internal::trivial< false > * )
      {
         return g.insert< Name >( rule_type::ALWAYS_CONSUMES_WHEN_SUCCEEDS ).first->first;
      }

      template< typename Name, typename Cond >
      std::string insert_rule( grammar_info & g, const internal::until< Cond > * )
      {
         return insert_help< Name, Cond, internal::any< char > >( g, rule_type::RULE_UNTIL );
      }

      template< typename Name, typename Cond, typename Rule, typename ... Rules >
      std::string insert_rule( grammar_info & g, const internal::until< Cond, Rule, Rules ... > * )
      {
         return insert_help< Name, Cond, Rule, Rules ... >( g, rule_type::RULE_UNTIL );
      }

   } // analysis

} // pegtl

#endif
