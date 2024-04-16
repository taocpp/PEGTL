// Copyright (c) 2018-2024 Dr. Colin Hirsch and Daniel Frey
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#if !defined( TAO_PEGTL_INT_NAME )
#error Missing name!
#endif

#if !defined( TAO_PEGTL_INT_TYPE )
#error Missing type!
#endif

#if !defined( TAO_PEGTL_NAMESPACE )
#error Missing namespace!
#endif

#define TAO_PEGTL_INT_IMPL( PeeK, NaMe ) PeeK##NaMe
#define TAO_PEGTL_INT_HELP( PeeK, NaMe ) TAO_PEGTL_INT_IMPL( PeeK, NaMe )

#define TAO_PEGTL_INT_PEEK TAO_PEGTL_INT_HELP( internal::peek_, TAO_PEGTL_INT_NAME )
#define TAO_PEGTL_INT_MASK TAO_PEGTL_INT_HELP( internal::peek_mask_, TAO_PEGTL_INT_NAME )

// What we really need here are namespace templates!

namespace TAO_PEGTL_NAMESPACE::TAO_PEGTL_INT_NAME
{
   // clang-format off
   struct any : internal::any< TAO_PEGTL_INT_PEEK > {};
   template< std::size_t Count > struct many : internal::many< Count, TAO_PEGTL_INT_PEEK > {};

   template< TAO_PEGTL_INT_TYPE... Cs > struct not_one : internal::not_one< TAO_PEGTL_INT_PEEK, Cs... > {};
   template< TAO_PEGTL_INT_TYPE Lo, TAO_PEGTL_INT_TYPE Hi > struct not_range : internal::not_range< TAO_PEGTL_INT_PEEK, Lo, Hi > {};
   template< TAO_PEGTL_INT_TYPE... Cs > struct one : internal::one< TAO_PEGTL_INT_PEEK, Cs... > {};
   template< TAO_PEGTL_INT_TYPE Lo, TAO_PEGTL_INT_TYPE Hi > struct range : internal::range< TAO_PEGTL_INT_PEEK, Lo, Hi > {};
   template< TAO_PEGTL_INT_TYPE... Cs > struct ranges : internal::ranges< TAO_PEGTL_INT_PEEK, Cs... > {};
   template< TAO_PEGTL_INT_TYPE... Cs > struct string : internal::seq_one< TAO_PEGTL_INT_PEEK, Cs... > {};

   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE... Cs > struct mask_not_one : internal::not_one< TAO_PEGTL_INT_MASK< M >, Cs... > {};
   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE Lo, TAO_PEGTL_INT_TYPE Hi > struct mask_not_range : internal::not_range< TAO_PEGTL_INT_MASK< M >, Lo, Hi > {};
   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE... Cs > struct mask_one : internal::one< TAO_PEGTL_INT_MASK< M >, Cs... > {};
   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE Lo, TAO_PEGTL_INT_TYPE Hi > struct mask_range : internal::range< TAO_PEGTL_INT_MASK< M >, Lo, Hi > {};
   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE... Cs > struct mask_ranges : internal::ranges< TAO_PEGTL_INT_MASK< M >, Cs... > {};
   template< TAO_PEGTL_INT_TYPE M, TAO_PEGTL_INT_TYPE... Cs > struct mask_string : internal::seq_one< TAO_PEGTL_INT_MASK< M >, Cs... > {};
   // clang-format on

}  // namespace TAO_PEGTL_NAMESPACE::TAO_PEGTL_INT_NAME

#undef TAO_PEGTL_INT_NAME
#undef TAO_PEGTL_INT_TYPE
#undef TAO_PEGTL_INT_IMPL
#undef TAO_PEGTL_INT_HELP
#undef TAO_PEGTL_INT_PEEK
#undef TAO_PEGTL_INT_MASK
