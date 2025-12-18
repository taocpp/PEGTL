# PEGTL Documentation

* [Main Page](https://github.com/taocpp/PEGTL)


## Pages

* [Introduction](Introduction.md)
* [Install Guide](Install-Guide.md)

* [Rules and Grammars](Rules-and-Grammars.md)
* [Inputs and Parsing](Inputs-and-Parsing.md) -- big TODO!
* [Actions and States](Actions-and-States.md)
* [Control and Debug](Control-and-Debug.md)
* [Errors and Exceptions](Errors-and-Exceptions.md) -- big TODO!

* [Rule Reference](Rule-Reference.md)
* [Input Reference](Input-Reference.md)
* [Action Reference](Action-Reference.md)
* [Control Reference](Control-Reference.md)

* [Stream Parsing](Stream-Parsing.md)
* [Advanced Topics](Advanced-Topics.md)

* [Migration Guide](Migration-Guide.md)
* [Changelog](Changelog.md)


## Contents

* [Introduction](Introduction.md)
  * [Hello, World!](Introduction.md#hello-world-)
  * [Default Namespace](Introduction.md#default-namespace)
  * [Parsing Expression Grammars](Introduction.md#parsing-expression-grammars)
  * [Essential Terminology](Introduction.md#essential-terminology)
* [Install Guide](Installing-Guide.md)
  * [Requirements](Install-Guide.md#requirements)
  * [Disabling Exceptions](Install-Guide.md#disabling-exceptions)
  * [Disabling RTTI](Install-Guide.md#disabling-rtti)
  * [Installation Packages](Install-Guide.md#installation-packages)
  * [Using Vcpkg](Install-Guide.md#using-vcpkg)
  * [Using Conan](Install-Guide.md#using-conan)
  * [Using CMake](Install-Guide.md#using-cmake)
    * [CMake Installation](Install-Guide.md#cmake-installation)
    * [`find_package`](Install-Guide.md#find_package)
    * [`add_subdirectory`](Install-Guide.md#add_subdirectory)
    * [Mixing `find_package` and `add_subdirectory`](Install-Guide.md#mixing-find_package-and-add_subdirectory)
  * [Manual Installation](Install-Guide.md#manual-installation)
  * [Embedding the PEGTL](Install-Guide.md#embedding-the-pegtl)
    * [Embedding in Binaries](Install-Guide.md#embedding-in-binaries)
    * [Embedding in Libraries](Install-Guide.md#embedding-in-libraries)
    * [Embedding in Library Interfaces](Install-Guide.md#embedding-in-library-interfaces)

* [Stream Parsing](Stream-Parsing.md)
  * [Overview](Stream-Parsing.md#overview)
  * [Buffers](Stream-Parsing.md#buffers)
    * [Alloc Buffer](Stream-Parsing.md#alloc-buffer)
    * [Array Buffer](Stream-Parsing.md#array-buffer)
    * [Other Buffer](Stream-Parsing.md#other-buffer)
  * [Readers](Stream-Parsing.md#readers)
    * [CStream Reader](Stream-Parsing.md#cstream-reader)
    * [CString Reader](Stream-Parsing.md#cstring-reader)
    * [IStream Reader](Stream-Parsing.md#istream-reader)
    * [Iterator Reader](Stream-Parsing.md#iterator-reader)
  * [Inputs](Stream-Parsing.md#inputs)
    * [Plain Inputs](Stream-Parsing.md#plain-inputs)
    * [Plain Auto Inputs](Stream-Parsing.md#plain-auto-inputs)
    * [Text Inputs](Stream-Parsing.md#text-inputs)
    * [Text Auto Inputs](Stream-Parsing.md#text-auto-inputs)
  * [Rules](Stream-Parsing.md#rules)
    * [`discard`](Stream-Parsing.md#discard)
    * [`is_stream`](Stream-Parsing.md#is_stream)
    * [`require< Num >`](Stream-Parsing.md#require-num-)
  * [Actions](Stream-Parsing.md#actions)
    * [`discard_input`](Stream-Parsing.md#discard_input)
    * [`discard_input_on< Bool >`](Stream-Parsing.md#discard_input_on-bool-)
    * [`discard_input_on_failure`](Stream-Parsing.md#discard_input_on_failure)
    * [`discard_input_on_success`](Stream-Parsing.md#discard_input_on_success)
* [Advanced Topics](Advanced-Topics.md)
  * [Grammar Analysis](Advanced-Topics.md#grammar-analysis)
    * [How To](Advanced-Topics.md#how-to)
    * [Example](Advanced-Topics.md#example)
    * [Requirements](Advanced-Topics.md#requirements)
    * [Limitations](Advanced-Topics.md#limitations)
  * [Optimizations](Advanced-Topics.md#optimizations)
    * [Backtracking](Advanced-Topics.md#backtracking)
    * [Whitespace](Advanced-Topics.md#whitespace)
  * [Development](Advanced-Topics.md#development)
* [Migration Guide](Migration-Guide.md)
  * [Version 4.0.0](Migration-Guide.md#version-400)
  * [Version 3.0.0](Migration-Guide.md#version-300)
  * [Version 2.8.0](Migration-Guide.md#version-280)
  * [Version 2.4.0](Migration-Guide.md#version-240)
  * [Version 2.0.0](Migration-Guide.md#version-200)
  * [Version 1.0.0](Migration-Guide.md#version-100)
* [Changelog](Changelog.md)


---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
