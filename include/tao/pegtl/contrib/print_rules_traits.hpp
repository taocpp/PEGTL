// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP
#define TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP

#include <ostream>

#include "../config.hpp"
#include "../rules.hpp"

#include "../internal/peek_char.hpp"
#include "../internal/peek_utf8.hpp"

#include "forward.hpp"

#include "print_basic_traits.hpp"

#include "internal/print_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template<>
   struct print_rules_traits< void >
      : print_basic_traits< void >
   {
      using print_basic_traits< void >::print_basic_traits;
   };

   template< typename Rule >
   struct print_rules_traits< Rule >
      : print_basic_traits< Rule >
   {};

   template< template< typename... > class Action, typename Rule >
   struct print_basic_traits< internal::action< Action, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "action" );
      }
   };

   template< template< typename... > class Action, typename... Rules >
   struct print_basic_traits< internal::action< Action, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "action" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "alnum" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "alpha" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::at< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "at" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::at< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "at" );
      }
   };

   template<>
   struct print_rules_traits< internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\t' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "blank" );
      }
   };

   template< template< typename... > class Control, typename Rule >
   struct print_basic_traits< internal::control< Control, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "control" );
      }
   };

   template< template< typename... > class Control, typename... Rules >
   struct print_basic_traits< internal::control< Control, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "control" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "digit" );
      }
   };

   template< typename Rule >
   struct print_basic_traits< internal::disable< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "disable" );
      }
   };

   template< typename... Rules >
   struct print_basic_traits< internal::disable< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "disable" );
      }
   };

   template<>
   struct print_rules_traits< internal::string< '.', '.', '.' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "ellipsis" );
      }
   };

   template< typename Rule >
   struct print_basic_traits< internal::enable< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "enable" );
      }
   };

   template< typename... Rules >
   struct print_basic_traits< internal::enable< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "enable" );
      }
   };

   template< char C, char... Cs >
   struct print_rules_traits< internal::rep< 42, internal::one< internal::result_on_found::success, internal::peek_char, C, Cs... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "forty_two" ) << "( ";
         ( pc.escape1( os, C ), ..., ( os << ", ", pc.escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template<>
   struct print_rules_traits< internal::seq< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '_' >, internal::star< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "identifier" );
      }
   };

   // TODO: identifier_first?
   // TODO: identifier_other?

   template< char... Cs >
   struct print_rules_traits< internal::istring< Cs... > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "istring" ) << "( \"";
         ( pc.escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::seq< internal::string< Cs... >, internal::not_at< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9', '_' > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "keyword" ) << "( \"";
         ( pc.escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, Rule > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep >( os, "list" );
      }
   };

   template< typename Rule, typename Sep, typename Pad >
   struct print_rules_traits< internal::seq< Rule, internal::star< internal::seq< internal::star< Pad >, Sep, internal::star< Pad > >, Rule > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep, Pad >( os, "list" );
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, internal::must< Rule > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep >( os, "list_must" );
      }
   };

   template< typename Rule, typename Sep, typename Pad >
   struct print_rules_traits< internal::seq< Rule, internal::star< internal::seq< internal::star< Pad >, Sep, internal::star< Pad > >, internal::must< Rule > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Sep, Pad >( os, "list_must" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "lower" );
      }
   };

   template< typename Head, typename Rule >
   struct print_rules_traits< internal::rematch< Head, internal::not_at< Rule, internal::eof > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Head, Rule >( os, "minus" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::not_at< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "not_at" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::not_at< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "not_at" );
      }
   };

   template< typename Peek >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, typename Peek::data_t( 0 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "nul" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::opt< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "opt" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::opt< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "opt" );
      }
   };

   template< typename Rule, typename Pad >
   struct print_rules_traits< internal::seq< internal::star< Pad >, Rule, internal::star< Pad > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad >( os, "pad" );
      }
   };

   template< typename Rule, typename Pad1, typename Pad2 >
   struct print_rules_traits< internal::seq< internal::star< Pad1 >, Rule, internal::star< Pad2 > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad1, Pad2 >( os, "pad" );
      }
   };

   template< typename Rule, typename Pad >
   struct print_rules_traits< internal::seq< internal::star< Pad >, internal::opt< Rule, internal::star< Pad > > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule, Pad >( os, "pad_opt" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::plus< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "plus" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::plus< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "plus" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, char( 32 ), char( 126 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "print" );
      }
   };

   template< unsigned Cnt, typename Rule >
   struct print_rules_traits< internal::rep< Cnt, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep" ) << "( " << Cnt;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep< Cnt, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep" ) << "( " << Cnt;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Max, typename Rule >
   struct print_rules_traits< internal::rep_min_max< 0, Max, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_max" ) << "( " << Max;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Max, typename... Rules >
   struct print_rules_traits< internal::rep_min_max< 0, Max, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_max" ) << "( " << Max;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Min, typename... Rules >
   struct print_rules_traits< internal::seq< internal::rep< Min, Rules... >, internal::star< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_min" ) << "( " << Min;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Min, unsigned Max, typename Rule >
   struct print_rules_traits< internal::rep_min_max< Min, Max, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_min_max" ) << "( " << Min << ", " << Max;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Min, unsigned Max, typename... Rules >
   struct print_rules_traits< internal::rep_min_max< Min, Max, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_min_max" ) << "( " << Min << ", " << Max;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename Rule >
   struct print_rules_traits< internal::rep_opt< Cnt, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_opt" ) << "( " << Cnt;
         pc.template print_list< Traits, Rule >( os, nullptr, ", " );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep_opt< Cnt, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "rep_opt" ) << "( " << Cnt;
         pc.template print_list< Traits, Rules... >( os, nullptr, ", " );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::seq< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, char( 0 ), char( 127 ) > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "seven" );
      }
   };

   template<>
   struct print_rules_traits< internal::seq< internal::string< '#', '!' >, internal::until< internal::eolf > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "shebang" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::sor< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template<>
   struct print_rules_traits< internal::one< internal::result_on_found::success, internal::peek_char, ' ', '\n', '\r', '\t', '\v', '\f' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "space" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::star< Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rule >( os, "star" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::star< internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "star" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::star< internal::if_must< false, Cond, Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Rules... >( os, "star_must" );
      }
   };

   template< char C >
   struct print_rules_traits< internal::string< C, C, C > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "three" ) << "( ";
         pc.escape1( os, C );
         os << " )";
      }
   };

   template< char C >
   struct print_rules_traits< internal::string< C, C > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "two" ) << "( ";
         pc.escape1( os, C );
         os << " )";
      }
   };

   template< typename Cond >
   struct print_rules_traits< internal::until< Cond > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond >( os, "until" );
      }
   };

   template< typename Cond, typename Rule >
   struct print_rules_traits< internal::until< Cond, Rule > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rule >( os, "until" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::until< Cond, internal::seq< Rules... > > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         pc.template print_list< Traits, Cond, Rules... >( os, "until" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "upper" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, '0', '9', 'a', 'f', 'A', 'F' > >
   {
      template< template< typename... > class Traits, typename Config >
      static void print( std::ostream& os, const Config& pc )
      {
         os << pc.pegtl( "xdigit" );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
