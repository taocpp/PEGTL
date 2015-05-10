-- premake5.lua

solution "PEGTL"
    location "build"
    includedirs { "." }
    kind "ConsoleApp"
    language "C++"
    defines { "_CRT_SECURE_NO_WARNINGS" }
    flags { "MultiProcessorCompile" }
    configurations { "Debug", "Release" }
    filter "configurations:Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
    filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"

project "pegtl"
    files { "pegtl/**.hh" }

group "Test"
project "UnitTest"
    files { "unit_tests/**.hh", "unit_tests/**.cc" }
    pchheader "test.hh"
    pchsource "unit_tests/trace.cc"

examples = {
    "calculator",
    "hello_world",
    "json_build_one",
    "json_build_two",
    "json_parse",
    "lua53_parse",
    "modulus_match",
    "s_expression",
    "sum",
    "unescape",
    "uri_trace"
    }
group ("Examples")
for _,proj in next,examples,nil do
    project (proj)
       files { "examples/" .. proj .. ".cc" }
end
