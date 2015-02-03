// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_ANALYSIS_INSERT_RULE_HH
#define PEGTL_ANALYSIS_INSERT_RULE_HH

#include "rule_class.hh"
#include "insert_help.hh"
#include "grammar_info.hh"

namespace pegtl
{
   namespace analysis
   {
      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const consumes< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CONSUMES );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const optional< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::OPTIONAL );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const conjunction< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::CONJUNCTION );
      }

      template< typename Name, typename ... Rules >
      std::string insert_rule( grammar_info & g, const disjunction< Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, rule_type::DISJUNCTION );
      }

      template< typename Name, unsigned Count >
      std::string insert_rule( grammar_info & g, const counting< Count > * )
      {
         return insert_help< Name >( g, Count ? rule_type::CONSUMES : rule_type::OPTIONAL );
      }

      template< typename Name, unsigned Count, typename ... Rules >
      std::string insert_rule( grammar_info & g, const repeating< Count, Rules ... > * )
      {
         return insert_help< Name, Rules ... >( g, Count ? rule_type::CONJUNCTION : rule_type::OPTIONAL );
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
