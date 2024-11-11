# Optimisation

Notes on performance characteristics and optimisation possibilities.

###### Backtracking

For performance reasons a grammar should be designed to minimise backtracking.
We will start with a simple example.

```c++
using namespace tao::pegtl;
struct R = sor< seq< A, B >, seq< A, C > > {};  // R = (AB)/(AC)
```

If the input matches `seq< A, C >`, then matching `R` on said input will parse `A` twice (assuming that `B` does not match anything that `C` does).
The first time `A` will match successfully during the unsuccessful attempt to match `seq< A, B >`.
The second time `A` will match the same part of the input successfully again during the successful attempt to match `seq< A, C >`.
The solution is to change the grammar as follows.

```c++
struct R = seq< A, sor< B, C > > {};  // R = A(B/C)
```

Not backtracking over `A` has the additional advantage of not triggering any action attached to `A` twice.

In practice, opportunities to remove superfluous backtracking might not be as obvious as with such a simple rule.
For a more complex example please read the comments ate the beginning of the Lua 5.3 grammar in `src/example/pegtl/lua53.hpp`.
It shows how to eliminate both left-recursion and superfluous backtracking with multiple rules and recursions.

###### Whitespace etc.

Grammars should be designed to minimise redundant multiple parsing of the same whitespace, comments or other padding.

One good way to achieve this is to choose a strategy for whitespace handling and then consistently stick to it.
For example the JSON grammar in `include/tao/pegtl/contrib/json.hpp` consistently has every rule for a "token" consume any following whitespace via the `ws` rule, too.
That way every rule can assume to start matching some "real" input since any whitespace would have already been consumed by the previous one.

###### Regarding `at` and `one`

The `at<>`-rule never consumes input, and therefore always uses an input-marker to rewind the input back to where it started, regardless of the match-result.
In the context of optimising our [JSON library](https://github.com/taocpp/json), we noticed that the combination `at< one< ... > >` could be combined into an optimised `at_one< ... >` rule:
Instead of `one< ... >` advancing the input, and `at< one< ... > >` rewinding, the combined rule would omit both the advancing and the rewinding.

Put to the test, the optimised `at_one< '"' >` rule did not show any performance advantage over `at< one< '"' > >`, at least with `-O3`.
Presumably the compiler was smart enough to perform the optimisation by itself.

However with `-O0`, the optimised `at_one< '"' >` was faster by 5-10% in a [JSON library](https://github.com/taocpp/json) micro-benchmark.
As the PEGTL should only be used with optimizations enabled, we removed the `at_one<>` rule, as we try to reduce the number of rules that don't provide a clear benefit.

We still need to test whether the compiler manages to perform the same optimisation in more complex cases.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2017-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
