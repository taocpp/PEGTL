# Object Parsing

The PEGTL can parse more than just sequences of characters, it can also parse binary data and arbitrary objects.


## Contents

* [Binary and Enums](#binary-and-enums)
* [Arbitrary Objects](#arbitrary-objects)


## Binary and Enums

There are [rules dedicated](Rule-Reference.md#binary) to parsing binary data in the form of integers and enums.


## Arbitrary Objects

There are [rules dedicated](Rule-Reference.md#member) to parsing arbitrary objects by applying predicates to their members.

See the two example programs `src/example/token_input_1.cpp` and `src/example/token_input_2.cpp` for how to put everything together.


---

This page is part of the [PEGTL](https://github.com/taocpp/PEGTL) and its [documentation](README.md).

Copyright (c) 2026 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
