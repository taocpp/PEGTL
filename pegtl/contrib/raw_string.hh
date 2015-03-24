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
         { }

         template< typename Input, typename ... States >
         void success( const Input &, States && ... ) const
         { }

         raw_string_state( const raw_string_state & ) = delete;
         void operator= ( const raw_string_state & ) = delete;

         // count is not initialised here because in a correct
         // grammar it will be first written by raw_string_open
         // before being read by raw_string_at_close.

         std::size_t count;
      };

      template< char Open, char Intermediate >
      struct raw_string_open
      {
         using analyze_t = analysis::generic< analysis::rule_type::ANY >;

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input >
         static bool match( Input & in, raw_string_state & ls )
         {
            if ( in.empty() || ( in.peek_char( 0 ) != Open ) ) {
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

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input >
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

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input >
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

      // We do not skip_control for raw_string_content since the user
      // will probably attach an action to get at the string's content.
      // For convenience, below the rule is imported into raw_string
      // with a using declaration, so the action specialisation can be
      // made for pegtl::raw_string::content.

      template< char Intermediate, char Close >
      struct raw_string_content : until< raw_string_at_close< Intermediate, Close > > {};

   } // internal

   // raw_string matches Lua-style long literals.
   //
   // The following description was taken from the Lua documentation
   // (see http://www.lua.org/docs.html):
   //
   // - An "opening long bracket of level n" is defined as an opening square
   //   bracket followed by n equal signs followed by another opening square
   //   bracket. So, an opening long bracket of level 0 is written as `[[`,
   //   an opening long bracket of level 1 is written as `[=[`, and so on.
   // - A "closing long bracket" is defined similarly; for instance, a closing
   //   long bracket of level 4 is written as `]====]`.
   // - A "long literal" starts with an opening long bracket of any level and
   //   ends at the first closing long bracket of the same level. It can
   //   contain any text except a closing bracket of the same level.
   // - Literals in this bracketed form can run for several lines, do not
   //   interpret any escape sequences, and ignore long brackets of any other
   //   level.
   // - For convenience, when the opening long bracket is immediately followed
   //   by a newline, the newline is not included in the string.
   //
   // Note that unlike Lua's long literal, a raw_string is customizable to use
   // other characters than `[`, `=` and `]` for matching. Also note that Lua
   // introduced newline-specific replacements in Lua 5.2, which we do not
   // support on the grammar level.

   template< char Open, char Intermediate, char Close >
   struct raw_string : state< internal::raw_string_state,
                              internal::raw_string_open< Open, Intermediate >,
                              internal::opt< internal::one< internal::result_on_found::SUCCESS, internal::peek_char, '\n' > >,
                              internal::must< internal::raw_string_content< Intermediate, Close > >,
                              internal::raw_string_bump_close >
   {
      using content = internal::raw_string_content< Intermediate, Close >;
   };

} // pegtl

#endif
