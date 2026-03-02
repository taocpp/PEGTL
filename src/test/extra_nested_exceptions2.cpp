// Copyright (c) 2024-2026 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( __cpp_exceptions ) || !defined( __cpp_rtti )
#include <iostream>
int main()
{
   std::cout << "Exception and/or RTTI support disabled, skipping test..." << std::endl;
}
#else

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
   const std::string s1 = "foo";
   const std::string s2 = "bar";

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
   void inspect( Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( std::current_exception(), visitor );
   }

   template< typename... Exceptions, typename Visitor >
   void inspect( const std::exception_ptr& ptr, Visitor&& visitor )
   {
      internal::inspector< Exceptions... >::inspect( ptr, visitor );
   }

   struct visitor
   {
      template< typename E >
      void operator()( const E& /*unused*/, const std::size_t /*unused*/ ) const noexcept
      {}
   };

}  // namespace

int main()
{
   try {
      try {
         throw std::string( s1 );
      }
      catch( ... ) {
         std::throw_with_nested( std::runtime_error( s2 ) );
      }
   }
   catch( ... ) {
      visitor v;
      inspect( v );
   }
   return 0;
}
