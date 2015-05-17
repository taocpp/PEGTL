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
    kind "None"
    files { "pegtl/**.hh" }

group "UnitTest"    
    for _,proj in next,os.matchfiles("unit_tests/*.cc"),nil do
        for name in string.gmatch(proj, "/([%a%d_]+)\.") do
            project(name)
            files { proj }
        end
    end

    project "include"
        kind "None"
        files { "unit_tests/**.hh" }

group ("Examples")
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
    for _,proj in next,examples,nil do
        project (proj)
           files { "examples/" .. proj .. ".cc" }
    end
