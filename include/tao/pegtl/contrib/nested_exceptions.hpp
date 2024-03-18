// Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_CONTRIB_NESTED_EXCEPTIONS_HPP
#define TAO_PEGTL_CONTRIB_NESTED_EXCEPTIONS_HPP

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#error "Exception and RTTI support required for tao/pegtl/contrib/nested_exceptions.hpp"
#endif

#include <exception>
#include <vector>

#include "../config.hpp"
#include "../parse_error.hpp"

// Some utility functions and classes to handle nested exceptions given that parse_nested() now uses them.
// At this point it's not clear yet how many, and which, of these will become and stay part of the PEGTL.

namespace TAO_PEGTL_NAMESPACE
{
   namespace internal
   {
      template< typename... >
      struct rethrower;

      template<>
      struct rethrower<>
      {
         template< typename Processor, typename Caught, typename Visitor >
         static void rethrow( const Caught& caught, Visitor&& /*unused*/, const std::size_t /*unused*/ )
         {
            std::rethrow_if_nested( caught );  // For the other/inner exceptions.
         }

         template< typename Processor, typename Visitor >
         static void rethrow( const std::exception_ptr& caught, Visitor&& /*unused*/, const std::size_t /*unused*/ )
         {
            std::rethrow_exception( caught );  // For the first/outer exception when supplied as exception pointer.
         }
      };

      template< typename Exception, typename... Exceptions >
      struct rethrower< Exception, Exceptions... >
      {
         template< typename Processor, typename Caught, typename Visitor >
         static void rethrow( const Caught& caught, Visitor&& visitor, const std::size_t level )
         {
            try {
               rethrower< Exceptions... >::template rethrow< Processor >( caught, visitor, level );
            }
            catch( const Exception& exception ) {
               Processor::process( exception, visitor, level );
            }
         }
      };

      template< typename Rethrower >
      struct processor
      {
         template< typename Exception, typename Visitor >
         static void process( const Exception& exception, Visitor&& visitor, const std::size_t level )
         {
            Rethrower::template rethrow< processor >( exception, visitor, level + 1 );
            visitor( exception, level );
         }
      };

      template< typename... Exceptions >
      struct inspector
      {
         using Rethrower = rethrower< Exceptions... >;
         using Processor = processor< Rethrower >;

         template< typename Visitor >
         static void inspect( const std::exception_ptr& ptr, Visitor&& visitor )
         {
            Rethrower::template rethrow< Processor >( ptr, visitor, 0 );
         }

         template< typename Exception, typename Visitor >
         static void inspect( const Exception& exception, Visitor&& visitor )
         {
            Processor::template process( exception, visitor, 0 );
         }
      };

   }  // namespace internal

   // Exceptions in an inheritance hierarchy MUST be listed FROM GENERAL TO SPECIFIC,
   // e.g. inspect< std::exception, std::runtime_error >, otherwise the more specific
   // exceptions will be erroneously caught by the handler for the more general case.

   // The visitor is called once for every exception, including the first/outer one.
   // The visitor is called first with the inner-most exception, i.e. the one that
   // was thrown first from furthest down/inside the call stack.
   // The visitor is called last with the outer-most exception, i.e. the one that was
   // caught in the try-catch that is calling inspect().

   // The second argument to the visitor, the std::size_t with the nesting level of
   // the exception that is passed as first argument, counts down from the inner-most
   // to the outer-most exception, i.e. it counts how deep down the current exception
   // is along the singly-linked list formed by the nested exceptions.

   // For example, when an exception E is caught, and E has exception F as nested
   // exception, then inspect( E, V ) for some visitor V will first call V( F, 1 )
   // and then V( E, 0 ).

   template< typename... Exceptions, typename Visitor >
   void inspect( Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( std::current_exception(), visitor );
   }

   template< typename... Exceptions, typename Visitor >
   void inspect( const std::exception_ptr& ptr, Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( ptr, visitor );
   }

   // NOTE: The following overload passes the outer exception to the visitor with its
   // static type, Exception! This might not be the expected or desired behaviour...

   template< typename... Exceptions, typename Exception, typename Visitor >
   void inspect( const Exception& exception, Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( exception, visitor );
   }

   // The visitor is supplied as last argument to the inspect() functions because it
   // seems to be a good idea to have lambda expressions as last argument, cf. Swift.

   [[nodiscard]] inline std::vector< parse_error_base > flatten( const std::exception_ptr& ptr = std::current_exception() )
   {
      std::vector< parse_error_base > result;
      inspect< parse_error_base >( ptr, [ &result ]( const parse_error_base& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e );
      } );
      return result;
   }

   [[nodiscard]] inline std::vector< parse_error_base > flatten( const parse_error_base& exception )
   {
      std::vector< parse_error_base > result;
      inspect< parse_error_base >( exception, [ &result ]( const parse_error_base& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e );
      } );
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
