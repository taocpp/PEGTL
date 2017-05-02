# Performance Notes

This page collects some notes and hints, observations and comments regarding the performance of the PEGTL.

###### Regarding `at` and `one`

The `at<>`-rule never consumes input, and therefore always uses an input-marker to rewind the input back to where it started, regardless of the match-result.
In the context of optimising our [JSON](https://github.com/taocpp/json), we noticed that the combination `at< one< ... > >` could be combined into an optimised `at_one< ... >` rule:
Instead of having `one< ... >` possibly advancing the input, and the `at< ... >` rewinding back, the combined rule could omit both the advancing and the rewinding.

We then put this idea to the test, pitching an optimised `at_one< '"' >` against the previous `at< one< '"' > >`.
The result is that, at least with some compilers and `-O3` optimisation levels, there is no measurable difference in performance.
At `-O0`, i.e. with optimisations disabled, the optimised `at_one< '"' >` was faster by 5-10% with some micro-benchmarks in the JSON library.
We conclude that the compiler is smart enough to notice which operations are superfluous, and that we do not need to implement this particular optimisation.

Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
