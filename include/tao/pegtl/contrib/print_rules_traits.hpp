// Copyright (c) 2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP
#define TAO_PEGTL_CONTRIB_PRINT_RULES_TRAITS_HPP

#include "../config.hpp"
#include "../rules.hpp"

#include "../internal/peek_char.hpp"
#include "../internal/peek_utf8.hpp"

#include "forward.hpp"

#include "internal/print_utility.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z', '0', '9' > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "alnum" );
      }
   };

   template<>
   struct print_rules_traits< internal::ranges< internal::peek_char, 'a', 'z', 'A', 'Z' > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "alpha" );
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_char > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "any" );
      }
   };

   template<>
   struct print_rules_traits< internal::any< internal::peek_utf8 > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "utf8" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::at< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "at" );
      }
   };

   template<>
   struct print_rules_traits< internal::bof >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "bof" );
      }
   };

   template<>
   struct print_rules_traits< internal::bol >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "bol" );
      }
   };

   template< unsigned Num >
   struct print_rules_traits< internal::bytes< Num > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "bytes" ) << "( " << Num << " )";
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, '0', '9' > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "digit" );
      }
   };

   template<>
   struct print_rules_traits< internal::eof >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "eof" );
      }
   };

   template<>
   struct print_rules_traits< internal::eol >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "eol" );
      }
   };

   template<>
   struct print_rules_traits< internal::eolf >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "eolf" );
      }
   };

   template<>
   struct print_rules_traits< internal::failure >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "failure" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< false, Cond, Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Cond, Rules... >( os, pc, "if_must" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, Then, Else > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Cond, Then, Else >( os, pc, "if_then_else" );
      }
   };

   template< typename Cond, typename Then, typename Else >
   struct print_rules_traits< internal::if_then_else< Cond, internal::must< Then >, internal::must< Else > > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Cond, Then, Else >( os, pc, "if_must_else" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::istring< Cs... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "istring" ) << "( \"";
         ( internal::print_escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, Rule > > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rule, Sep >( os, pc, "list" );
      }
   };

   template< typename Rule, typename Sep >
   struct print_rules_traits< internal::seq< Rule, internal::star< Sep, internal::must< Rule > > > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rule, Sep >( os, pc, "list_must" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'a', 'z' > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "lower" );
      }
   };

   template< typename Head, typename Rule >
   struct print_rules_traits< internal::rematch< Head, internal::not_at< Rule, internal::eof > > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Head, Rule >( os, pc, "minus" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::must< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "must" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::not_at< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "not_at" );
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::failure, Peek, C, Cs... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "not_one" ) << "( ";
         ( internal::print_escape1( os, C ), ..., ( os << ", ", internal::print_escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::failure, Peek, Lo, Hi > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "not_range" ) << "( ";
         internal::print_escape1( os, Lo );
         os << " - ";
         internal::print_escape1( os, Hi );
         os << " )";
      }
   };

   template< typename Peek, typename Peek::data_t C, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::one< internal::result_on_found::success, Peek, C, Cs... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "one" ) << "( ";
         ( internal::print_escape1( os, C ), ..., ( os << ", ", internal::print_escape1( os, Cs ) ) );
         os << " )";
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::opt< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "opt" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::if_must< true, Cond, Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Cond, Rules... >( os, pc, "opt_must" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::plus< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "plus" );
      }
   };

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi >
   struct print_rules_traits< internal::range< internal::result_on_found::success, Peek, Lo, Hi > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "range" ) << "( ";
         internal::print_escape1( os, Lo );
         os << " - ";
         internal::print_escape1( os, Hi );
         os << " )";
      }
   };

   namespace internal
   {
      template< typename T, T... >
      struct print_ranges_rules_traits;

      template< typename T, T C >
      struct print_ranges_rules_traits< T, C >
      {
         static void print( std::ostream& os )
         {
            os << ", ";
            internal::print_escape1( os, C );
         }
      };

      template< typename T, T Lo, T Hi, T... Ts >
      struct print_ranges_rules_traits< T, Lo, Hi, Ts... >
      {
         static void print( std::ostream& os )
         {
            os << ", ";
            internal::print_escape1( os, Lo );
            os << " - ";
            internal::print_escape1( os, Hi );
         }
      };

   }  // internal

   template< typename Peek, typename Peek::data_t Lo, typename Peek::data_t Hi, typename Peek::data_t... Cs >
   struct print_rules_traits< internal::ranges< Peek, Lo, Hi, Cs... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "ranges" ) << "( ";
         internal::print_escape1( os, Lo );
         os << " - ";
         internal::print_escape1( os, Hi );
         internal::print_ranges_rules_traits< typename Peek::data_t, Cs... >::print( os );
         os << " )";
      }
   };

   template< typename Head, typename... Rules >
   struct print_rules_traits< internal::rematch< Head, Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Head, Rules... >( os, pc, "rematch" );
      }
   };

   template< unsigned Cnt, typename... Rules >
   struct print_rules_traits< internal::rep< Cnt, Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "rep" ) << "( " << Cnt;
         internal::print_rules_rules< Rules... >( os, pc, nullptr, ", " );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::seq< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::seq< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "seq" );
      }
   };

   template< typename Rule >
   struct print_rules_traits< internal::sor< Rule > >
      : print_rules_traits< typename Rule::rule_t >
   {};

   template< typename... Rules >
   struct print_rules_traits< internal::sor< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "sor" );
      }
   };

   template< typename... Rules >
   struct print_rules_traits< internal::star< Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Rules... >( os, pc, "star" );
      }
   };

   template< char... Cs >
   struct print_rules_traits< internal::string< Cs... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "string" ) << "( \"";
         ( internal::print_escape( os, Cs ), ... );
         os << "\" )";
      }
   };

   template<>
   struct print_rules_traits< internal::success >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "success" );
      }
   };

   template< typename Cond, typename... Rules >
   struct print_rules_traits< internal::until< Cond, Rules... > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         internal::print_rules_rules< Cond, Rules... >( os, pc, "until" );
      }
   };

   template<>
   struct print_rules_traits< internal::range< internal::result_on_found::success, internal::peek_char, 'A', 'Z' > >
   {
      static void print( std::ostream& os, const internal::print_rules_config& pc )
      {
         os << pc.pegtl( "upper" );
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
