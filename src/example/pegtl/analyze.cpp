// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>

using namespace tao::TAO_PEGTL_NAMESPACE;  // NOLINT

struct bar;

struct foo
   : sor< digit, bar >
{
};

struct bar
   : plus< foo >
{
};

int main( int /*unused*/, char** /*unused*/ )
{
   if( analyze< foo >() != 0 ) {
      std::cout << "there are problems" << std::endl;
      return 1;
   }
   return 0;
}
