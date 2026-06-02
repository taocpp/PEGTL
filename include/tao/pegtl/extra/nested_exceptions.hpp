// Copyright (c) 2023-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef TAO_PEGTL_EXTRA_NESTED_EXCEPTIONS_HPP
#define TAO_PEGTL_EXTRA_NESTED_EXCEPTIONS_HPP

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#error "Exception and RTTI support required for tao/pegtl/extra/nested_exceptions.hpp"
#endif

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

#include "../config.hpp"
#include "../parse_error.hpp"

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
            Processor::process( exception, visitor, 0 );
         }
      };

   }  // namespace internal

   template< typename... Exceptions, typename Visitor >
   void visit_nested( Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( std::current_exception(), visitor );
   }

   template< typename... Exceptions, typename Visitor >
   void visit_nested( const std::exception_ptr& ptr, Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( ptr, visitor );
   }

   // NOTE: The following overload passes the outer exception to the visitor with its
   // static type, Exception! This might not be the expected or desired behavior...

   template< typename... Exceptions, typename Exception, typename Visitor >
   void visit_nested( const Exception& exception, Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( exception, visitor );
   }

   template< typename Exception >
   [[nodiscard]] std::vector< Exception > flatten_type( const std::exception_ptr& ptr = std::current_exception() )
   {
      std::vector< Exception > result;
      visit_nested< Exception >( ptr, [ &result ]( const Exception& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e );
      } );
      return result;
   }

   template< typename Exception >
   [[nodiscard]] std::vector< Exception > flatten_type( const Exception& exception )
   {
      std::vector< Exception > result;
      visit_nested< Exception >( exception, [ &result ]( const Exception& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e );
      } );
      return result;
   }

   [[nodiscard]] inline std::vector< parse_error_base > flatten_base( const std::exception_ptr& ptr = std::current_exception() )
   {
      return flatten_type< parse_error_base >( ptr );
   }

   [[nodiscard]] inline std::vector< parse_error_base > flatten_base( const parse_error_base& exception )
   {
      return flatten_type< parse_error_base >( exception );
   }

   [[nodiscard]] inline std::vector< std::string > flatten_what( const std::exception_ptr& ptr = std::current_exception() )
   {
      std::vector< std::string > result;
      visit_nested< std::exception >( ptr, [ &result ]( const std::exception& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e.what() );
      } );
      return result;
   }

   [[nodiscard]] inline std::vector< std::string > flatten_what( const std::exception& exception )
   {
      std::vector< std::string > result;
      visit_nested< std::exception >( exception, [ &result ]( const std::exception& e, const std::size_t /*unused*/ ) {
         result.emplace_back( e.what() );
      } );
      return result;
   }

}  // namespace TAO_PEGTL_NAMESPACE

#endif
