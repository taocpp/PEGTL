// Copyright (c) 2020-2025 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_DEBUG_TRACE_STRUCT_HPP
#define TAO_PEGTL_DEBUG_TRACE_STRUCT_HPP

#include <cstddef>
#include <iomanip>
#include <ostream>
#include <utility>
#include <vector>

#include "../config.hpp"
#include "../demangle.hpp"

namespace TAO_PEGTL_NAMESPACE
{
   template< typename TraceTraits, typename ErrorPosition >
   struct trace_struct
   {
      trace_struct( std::ostream& os, const ErrorPosition& pos )
         : flags( os.flags() ),
           stream( os ),
           position( pos )
      {
         stream << std::left;
         print_position();
      }

      trace_struct( trace_struct&& ) = delete;
      trace_struct( const trace_struct& ) = delete;

      ~trace_struct()
      {
         stream.flags( flags );
      }

      void operator=( trace_struct&& ) = delete;
      void operator=( const trace_struct& ) = delete;

      const std::ios_base::fmtflags flags;

      std::ostream& stream;
      std::size_t count = 0;
      std::vector< std::size_t > stack;
      ErrorPosition position;

      [[nodiscard]] std::size_t indent() const noexcept
      {
         return TraceTraits::initial_indent + TraceTraits::indent_increment * stack.size();
      }

      void print_position() const
      {
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_position << "position" << TraceTraits::ansi_reset << ' ' << position << '\n';
      }

      void update_position( const ErrorPosition& pos )
      {
         if( position != pos ) {
            position = pos;
            print_position();
         }
      }

      template< typename Rule >
      void start()
      {
         stream << '#' << std::setw( indent() - 1 ) << ++count << TraceTraits::ansi_rule << demangle< Rule >() << TraceTraits::ansi_reset << '\n';
         stack.push_back( count );
      }

      template< typename Rule >
      void success( const ErrorPosition& pos )
      {
         const auto prev = stack.back();
         stack.pop_back();
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_success << "success" << TraceTraits::ansi_reset;
         if( count != prev ) {
            stream << " #" << prev << ' ' << TraceTraits::ansi_hide << demangle< Rule >() << TraceTraits::ansi_reset;
         }
         stream << '\n';
         update_position( pos );
      }

      template< typename Rule >
      void failure( const ErrorPosition& pos )
      {
         const auto prev = stack.back();
         stack.pop_back();
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_failure << "failure" << TraceTraits::ansi_reset;
         if( count != prev ) {
            stream << " #" << prev << ' ' << TraceTraits::ansi_hide << demangle< Rule >() << TraceTraits::ansi_reset;
         }
         stream << '\n';
         update_position( pos );
      }

      template< typename Rule, typename RewindPosition >
      void prep_rewind( const RewindPosition& pos )
      {
         stream << std::setw( indent() ) << ' ' << "prep rewind " << pos << '\n';
      }

      template< typename Rule, typename RewindPosition >
      void will_rewind( const RewindPosition& pos )
      {
         stream << std::setw( indent() ) << ' ' << "will rewind " << pos << '\n';
      }

      template< typename Rule, typename RewindPosition >
      void wont_rewind( const RewindPosition& pos )
      {
         stream << std::setw( indent() ) << ' ' << "wont rewind " << pos << '\n';
      }

      template< typename Rule >
      void raise()
      {
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_raise << "raise" << TraceTraits::ansi_reset << ' ' << TraceTraits::ansi_rule << demangle< Rule >() << TraceTraits::ansi_reset << '\n';
      }

      template< typename Rule >
      void raise_nested()
      {
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_raise << "raise_nested" << TraceTraits::ansi_reset << ' ' << TraceTraits::ansi_rule << demangle< Rule >() << TraceTraits::ansi_reset << '\n';
      }

      template< typename Rule >
      void unwind( const ErrorPosition& pos )
      {
         const auto prev = stack.back();
         stack.pop_back();
         stream << std::setw( indent() ) << ' ' << TraceTraits::ansi_unwind << "unwind" << TraceTraits::ansi_reset;
         if( count != prev ) {
            stream << " #" << prev << ' ' << TraceTraits::ansi_hide << demangle< Rule >() << TraceTraits::ansi_reset;
         }
         stream << '\n';
         update_position( pos );
      }

      template< typename Rule >
      void apply()
      {
         stream << std::setw( static_cast< int >( indent() - TraceTraits::indent_increment ) ) << ' ' << TraceTraits::ansi_apply << "apply" << TraceTraits::ansi_reset << '\n';
      }

      template< typename Rule >
      void apply0()
      {
         stream << std::setw( static_cast< int >( indent() - TraceTraits::indent_increment ) ) << ' ' << TraceTraits::ansi_apply << "apply0" << TraceTraits::ansi_reset << '\n';
      }
   };

}  // namespace TAO_PEGTL_NAMESPACE

#endif
