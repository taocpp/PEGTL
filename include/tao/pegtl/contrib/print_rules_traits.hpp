// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP
#define TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP

#include "../config.hpp"
#include "../rules.hpp"

#include "forward.hpp"

#include "internal/print_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "alnum";
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "alpha";
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_char > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "any";
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_utf8 > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "utf8";
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::at< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "at" );
      }
   };

   template<>
   struct print_rules_traits< internal::bof >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "bof";
      }
   };

   template<>
   struct print_rules_traits< internal::bol >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "bol";
      }
   };

   template< unsigned Num >
   struct print_rules_traits< internal::bytes< Num > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "bytes( " << Num << " )";
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "digit";
      }
   };

   template<>
   struct print_rules_traits< internal::eof >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "eof";
      }
   };

   template<>
   struct print_rules_traits< internal::eol >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "eol";
      }
   };

   template<>
   struct print_rules_traits< internal::eolf >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "eolf";
      }
   };

   template<>
   struct print_rules_traits< internal::failure >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "failure";
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< false, Cond, Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Cond, Rules... >( os, prefix, "if_must" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, Then, Else > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Cond, Then, Else >( os, prefix, "if_then_else" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, internal::must< Then >, internal::must< Else > > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Cond, Then, Else >( os, prefix, "if_must_else" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::istring< Cs... > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "istring( \"";
         ( internal::print_escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, Rule > > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rule, Sep >( os, prefix, "list" );
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, internal::must< Rule > > > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rule, Sep >( os, prefix, "list_must" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "lower";
      }
   };

   template< typename Head, typename Rule >
   struct print_rules_traits< internal::rematch< Head, internal::not_at< Rule, internal::eof > > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Head, Rule >( os, prefix, "minus" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::must< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "\033[38;5;202mmust\033[m" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::not_at< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "not_at" );
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::failure, Peek, C, Cs... > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "not_one( ";
         ( internal::print_escape1( os, C ), ..., ( os << ", ", internal::print_escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::failure, Peek, Lo, Hi > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "not_range( ";
         internal::print_escape1( os, Lo );
         os << ", ";
         internal::print_escape1( os, Hi );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, C, Cs... > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "one( ";
         ( internal::print_escape1( os, C ), ..., ( os << ", ", internal::print_escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::opt< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "opt" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< true, Cond, Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Cond, Rules... >( os, prefix, "opt_must" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::plus< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "plus" );
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::success, Peek, Lo, Hi > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "range( ";
         internal::print_escape1( os, Lo );
         os << ", ";
         internal::print_escape1( os, Hi );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::ranges< Peek, C, Cs... > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "ranges( ";
         ( internal::print_escape1( os, C ), ..., ( os << ", ", internal::print_escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename Head, typename... Rules >
   struct print_rules_traits< internal::rematch< Head, Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Head, Rules... >( os, prefix, "rematch" );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep< Cnt, Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         os << "rep( " << Cnt;
         internal::print_rules_rules< Rules... >( os, prefix, "", ", " );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::seq< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::seq< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "\033[1;38;5;64mseq\033[m" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::sor< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::sor< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "\033[1;38;5;27msor\033[m" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::star< Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Rules... >( os, prefix, "star" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::string< Cs... > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "string( \"";
         ( internal::print_escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template<>
   struct print_rules_traits< internal::success >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "success";
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::until< Cond, Rules... > >
   {
      static void print( std::ostream& os, const std::string_view prefix )
      {
         internal::print_rules_rules< Cond, Rules... >( os, prefix, "until" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > >
   {
      static void print( std::ostream& os, const std::string_view /*unused*/ )
      {
         os << "upper";
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
