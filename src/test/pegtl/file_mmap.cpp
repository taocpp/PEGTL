// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl/file_parser.hpp>

#if defined(_POSIX_MAPPED_FILES)

#include "test.hpp"

#include "verify_file.hpp"

namespace tao
{
   namespace TAOCPP_PEGTL_NAMESPACE
   {
      void unit_test()
      {
         verify_file< mmap_parser >();
      }

   } // namespace TAOCPP_PEGTL_NAMESPACE

} // namespace tao

#include "main.hpp"

#else

int main( int, char ** )
{
   return 0;
}

#endif
