# set C++ language standard
set (CMAKE_CXX_EXTENSIONS OFF)

# add warnings
if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /utf-8")
else ()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wshadow -Werror")
endif ()

# add examples
file (GLOB examplesources *.cc)
foreach (examplesourcefile ${examplesources})
  get_filename_component (exename ${examplesourcefile} NAME_WE)
  add_executable (${exename} ${examplesourcefile})
  target_link_libraries (${exename} PEGTL)
endforeach (examplesourcefile)
