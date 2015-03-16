// Copyright (c) 2014-2015 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef PEGTL_CONTRIB_RAW_STRING_HH
#define PEGTL_CONTRIB_RAW_STRING_HH

#include "../apply_mode.hh"

#include "../internal/until.hh"
#include "../internal/state.hh"
#include "../internal/opt.hh"
#include "../internal/one.hh"
#include "../internal/skip_control.hh"

#include "../analysis/generic.hh"

namespace pegtl
{
   namespace internal
   {
      struct raw_string_state
      {
         template< typename Input, typename ... States >
         raw_string_state( const Input &, States && ... )
               : count( 0 )
         { }

         template< typename Input, typename ... States >
         void success( const Input &, States && ... ) const
         { }

         raw_string_state( const raw_string_state & ) = delete;
         void operator= ( const raw_string_state & ) = delete;

         std::size_t count;
      };

      template< char Open, char Intermediate >
      struct raw_string_open
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< apply_mode, template< typename ... > class, template< typename ... > class, typename Input >
         static bool match( Input & in, raw_string_state & ls )
         {
            if ( in.empty() || in.peek_char( 0 ) != Open ) {
               return false;
            }
            for ( std::size_t i = 1; i < in.size(); ++i ) {
               switch ( const auto c = in.peek_char( i ) ) {
                  case Open:
                     ls.count = i - 1;
                     in.bump( i + 1 );
                     return true;
                  case Intermediate:
                     break;
                  default:
                     return false;
               }
            }
            return false;
         }
      };

      template< char Intermediate, char Close >
      struct raw_string_at_close
      {
         using analyze_t = analysis::generic< analysis::rule_type::OPT >;

         template< apply_mode, template< typename ... > class, template< typename ... > class, typename Input >
         static bool match( Input & in, const raw_string_state & ls )
         {
            if ( in.size() < ls.count + 2 ) {
               return false;
            }
            if ( in.peek_char( 0 ) != Close ) {
               return false;
            }
            if ( in.peek_char( ls.count + 1 ) != Close ) {
               return false;
            }
            for ( std::size_t i = 0; i < ls.count; ++i ) {
               if ( in.peek_char( i + 1 ) != Intermediate ) {
                  return false;
               }
            }
            // Do not bump the input here (implicit at<>).
            return true;
         }
      };

      struct raw_string_bump_close
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< apply_mode, template< typename ... > class, template< typename ... > class, typename Input >
         static bool match( Input & in, const raw_string_state & ls )
         {
            in.bump( ls.count + 2 );
            return true;
         }
      };

      template< char Open, char Intermediate >
      struct skip_control< raw_string_open< Open, Intermediate > > : std::true_type {};

      template< char Intermediate, char Close >
      struct skip_control< raw_string_at_close< Intermediate, Close > > : std::true_type {};

      template<>
      struct skip_control< raw_string_bump_close > : std::true_type {};

      template< char Intermediate, char Close >
      struct raw_string_content : until< raw_string_at_close< Intermediate, Close > > {};

   } // internal

   template< char Open, char Intermediate, char Close >
   struct raw_string : state< internal::raw_string_state,
                              internal::raw_string_open< Open, Intermediate >,
                              internal::opt< internal::one< true, internal::peek_char, '\n' > >,
                              internal::must< internal::raw_string_content< Intermediate, Close > >,
                              internal::raw_string_bump_close >
   {
      using content = internal::raw_string_content< Intermediate, Close >;
   };

} // pegtl

#endif
