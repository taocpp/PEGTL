# set C++ language standard
set (CMAKE_CXX_EXTENSIONS OFF)

# add warnings
if (MSVC)
  add_compile_options (/W4 /WX /utf-8)
else ()
  add_compile_options (-pedantic -Wall -Wextra -Wshadow -Werror)
endif ()

# add examples
file (GLOB examplesources *.cpp)
foreach (examplesourcefile ${examplesources})
  get_filename_component (exename ${examplesourcefile} NAME_WE)
  add_executable (${exename} ${examplesourcefile})
  target_link_libraries (${exename} PEGTL)
endforeach (examplesourcefile)
