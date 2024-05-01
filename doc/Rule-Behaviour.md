# Rule Behaviour

These tables show how groups of related combinators differ by giving examples of their matching behaviour.

We assume that the rule `x` is defined as `one< 'x' >` for all reasonable choices of `x`.
We also assume that we are in namespace `tao::pegtl`.

In these tables a quoted string indicates that the rule matched on the input and shows which part of the input was consumed.
The letter 'f' stands for a "local failure" where the rule returns `false` while the letter 'E' is for a "global failure" where the rule throws an exception.

## Simple Combinators

|  | "" | "a" | "ab" | "z" | "az" |
|--|--|--|--|--|--|
| `seq< a, b >` | f | f | "ab" | f | f |
| `opt< a, b >` | "" | "" | "ab" | "" | "" |
| `strict< a, b >` | "" | f | "ab" | "" | f |
| `partial< a, b >` | "" | "a" | "ab" | "" | "a" |
| `if_must< a, b >` | f | E | "ab" | f | E |

## Repeating Combinators

|  | "" | "a" | "aa" | "aaa" | "aaaa" |
|--|--|--|--|--|--|
| `rep< 2, a >` | f | f | "aa" | "aa" | "aa" |
| `rep_opt< 2, a >` | "" | "a" | "aa" | "aa" | "aa" |
| `rep_min< 2, a >` | f | f | "aa" | "aaa" | "aaaa" |
| `rep_max< 2, a >` | "" | "a" | "aa" | f | f |
| `rep_min_max< 2, 3, a >` | f | f | "aa" | "aaa" | f |

## Iterating Combinators

|  | "" | "a" | "ab" | "aba" | "abab" | "z" | "az" | "abz" | "abaz" |
|--|--|--|--|--|--|--|--|--|--|
| `plus< a, b >` | f | f | "ab" | "ab" | "abab" | f | f | "ab" | "ab" |
| `star< a, b >` | "" | "" | "ab" | "ab" | "abab" | "" | "" | "ab" | "ab" |
| `star_strict< a, b >` | "" | f | "ab" | f | "abab" | "" | f | "ab" | f |
| `star_partial< a, b >` | "" | "a" | "ab" | "aba" | "abab" | "" | "a" | "ab" | "aba" |
| `star_must< a, b >` | "" | E | "ab" | E | "abab" | "" | E | "ab" | E |

## Rule List Combinators

|  | "a" | "aa" | "ab" | "aba" | "abaa" | "abab" | "ac" | "acb" | "acba" | "acbca" |
|--|--|--|--|--|--|--|--|--|--|--|
| `list< a, b >` | "a" | "a" | "a" | "aba" | "aba" | "aba" | "a" | "a" | "a" | "a" |
| `list_tail< a, b >` | "a" | "a" | "ab" | "aba" | "aba" | "abab" | "a" | "a" | "a" | "a" |
| `list_must< a, b >` | "a" | "a" | E | "aba" | "aba" | E | "a" | "a" | "a" | "a" |
| `list< a, b, c >` | "a" | "a" | "a" | "aba" | "aba" | "aba" | "a" | "a" | "acba" | "acbca" |
| `list_tail< a, b, c >` | "a" | "a" | "ab" | "aba" | "aba" | "abab" | "a" | "acb" | "acba" | "acbca" |
| `list_must< a, b, c >` | "a" | "a" | E | "aba" | "aba" | E | "a" | E | "acba" | "acbca" |

The list rules all fail (locally) when they can not match at least one list element.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
