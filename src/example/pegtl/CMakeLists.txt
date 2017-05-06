# set C++ language standard
set (CMAKE_CXX_EXTENSIONS OFF)

# add examples
file (GLOB examplesources *.cpp)
foreach (examplesourcefile ${examplesources})
  get_filename_component (exename ${examplesourcefile} NAME_WE)
  add_executable (${exename} ${examplesourcefile})
  if (MSVC)
    target_compile_options (${exename} /W4 /WX /utf-8)
  else ()
    target_compile_options (${exename} -pedantic -Wall -Wextra -Wshadow -Werror)
  endif ()
  target_link_libraries (${exename} pegtl)
endforeach (examplesourcefile)
