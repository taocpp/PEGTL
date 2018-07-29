// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <utility>

int main()
{
#ifdef __GNUC__
   std::cout << "__GNUC__: " << __GNUC__ << std::endl;
   std::cout << "__GNUC_MINOR__: " << __GNUC_MINOR__ << std::endl;
   std::cout << "__GNUC_PATCHLEVEL__: " << __GNUC_PATCHLEVEL__ << std::endl;
#endif
#ifdef __GLIBCPP__
   std::cout << "__GLIBCPP__: " << __GLIBCPP__ << std::endl;
#endif
#ifdef __GLIBCXX__
   std::cout << "__GLIBCXX__: " << __GLIBCXX__ << std::endl;
#endif

#ifdef __clang__
   std::cout << "__clang__: " << __clang__ << std::endl;
   std::cout << "__clang_major__: " << __clang_major__ << std::endl;
   std::cout << "__clang_minor__: " << __clang_minor__ << std::endl;
   std::cout << "__clang_patchlevel__: " << __clang_patchlevel__ << std::endl;
   std::cout << "__clang_version__: " << __clang_version__ << std::endl;
#endif
#ifdef __apple_build_version__
   std::cout << "__apple_build_version__: " << __apple_build_version__ << std::endl;
#endif
#ifdef _LIBCPP_VERSION
   std::cout << "_LIBCPP_VERSION: " << _LIBCPP_VERSION << std::endl;
#endif

#ifdef _MSC_VER
   std::cout << "_MSC_VER: " << _MSC_VER << std::endl;
   std::cout << "_MSC_FULL_VER: " << _MSC_FULL_VER << std::endl;
#endif

#ifdef __cplusplus
   std::cout << "__cplusplus: " << __cplusplus << std::endl;
#endif
#ifdef __cpp_constexpr
   std::cout << "__cpp_constexpr: " << __cpp_constexpr << std::endl;
#endif
#ifdef __cpp_variadic_templates
   std::cout << "__cpp_variadic_templates: " << __cpp_variadic_templates << std::endl;
#endif
#ifdef __cpp_variable_templates
   std::cout << "__cpp_variable_templates: " << __cpp_variable_templates << std::endl;
#endif
#ifdef __cpp_fold_expressions
   std::cout << "__cpp_fold_expressions: " << __cpp_fold_expressions << std::endl;
#endif
#ifdef __cpp_deduction_guides
   std::cout << "__cpp_deduction_guides: " << __cpp_deduction_guides << std::endl;
#endif
#ifdef __cpp_lib_integer_sequence
   std::cout << "__cpp_lib_integer_sequence: " << __cpp_lib_integer_sequence << std::endl;
#endif
}
