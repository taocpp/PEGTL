// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_UNREACHABLE_HPP
#define TAO_PEGTL_UNREACHABLE_HPP

#ifndef TAO_PEGTL_UNREACHABLE
#if defined( _MSC_VER )
#define TAO_PEGTL_UNREACHABLE __assume( false )
#else
#define TAO_PEGTL_UNREACHABLE __builtin_unreachable()
#endif
#endif

#endif
