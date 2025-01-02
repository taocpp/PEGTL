# Rule Behaviour

These tables show how groups of related combinators differ by giving examples of their respective matching behaviours.

In these tables a quoted string indicates that the rule matched on the input, and shows which part of the input was consumed.
The unquoted letter '**f**' stands for a "local failure" where the rule returns `false`.
The unquoted letter '**E**' stands for a "global failure" where the rule throws a `parse_error_base`-derived exception.

In order to keep the rules in the tables short we assume that we are in namespace `tao::pegtl`.
And that the rules `x` are defined as [`one< 'x' >`](Rule-Reference.md#one-c-) for all reasonable choices of `x`.
We also assume a byte-based input.

```c++
using namespace tao::pegtl;

struct a : one< 'a' > {};
struct b : one< 'b' > {};
struct c : one< 'c' > {};
```

## Simple Combinators

|  | "" | "a" | "ab" | "z" | "az" |
|--|--|--|--|--|--|
| [`seq< a, b >`](Rule-Reference.md#seq-r-) | **f** | **f** | "ab" | **f** | **f** |
| [`opt< a, b >`](Rule-Reference.md#opt-r-) | "" | "" | "ab" | "" | "" |
| [`opt_must< a, b >`](Rule-Reference.md#opt_must-r-) | "" | **E** | "ab" | "" | **E** |
| [`strict< a, b >`](Rule-Reference.md#strict-r-) | "" | **f** | "ab" | "" | **f** |
| [`partial< a, b >`](Rule-Reference.md#partial-r-) | "" | "a" | "ab" | "" | "a" |
| [`must< a, b >`](Rule-Reference.md#must-r-) | **E** | E | "ab" | **E** | E |
| [`if_must< a, b >`](Rule-Reference.md#if_must-r-s-) | **f** | **E** | "ab" | **f** | **E** |

## Iterating Combinators

|  | "" | "a" | "ab" | "aba" | "abab" | "z" | "az" | "abz" | "abaz" |
|--|--|--|--|--|--|--|--|--|--|
| [`plus< a, b >`](Rule-Reference.md#plus-r-) | **f** | **f** | "ab" | "ab" | "abab" | **f** | **f** | "ab" | "ab" |
| [`star< a, b >`](Rule-Reference.md#star-r-) | "" | "" | "ab" | "ab" | "abab" | "" | "" | "ab" | "ab" |
| [`star_strict< a, b >`](Rule-Reference.md#star_strict-r-) | "" | **f** | "ab" | **f** | "abab" | "" | **f** | "ab" | **f** |
| [`star_partial< a, b >`](Rule-Reference.md#star_partial-r-) | "" | "a" | "ab" | "aba" | "abab" | "" | "a" | "ab" | "aba" |
| [`star_must< a, b >`](Rule-Reference.md#star_must-r-s-) | "" | **E** | "ab" | **E** | "abab" | "" | **E** | "ab" | **E** |

## Repeating Combinators

|  | "" | "a" | "aa" | "aaa" | "aaaa" |
|--|--|--|--|--|--|
| [`rep< 2, a >`](Rule-Reference.md#rep-num-r-) | **f** | **f** | "aa" | "aa" | "aa" |
| [`rep_opt< 2, a >`](Rule-Reference.md#rep_opt-num-r-) | "" | "a" | "aa" | "aa" | "aa" |
| [`rep_min< 2, a >`](Rule-Reference.md#rep_min-min-r-) | **f** | **f** | "aa" | "aaa" | "aaaa" |
| [`rep_max< 2, a >`](Rule-Reference.md#rep_max-max-r-) | "" | "a" | "aa" | **f** | **f** |
| [`rep_min_max< 2, 3, a >`](Rule-Reference.md#rep_min_max-min-max-r-) | **f** | **f** | "aa" | "aaa" | **f** |

## List Combinators

|  | "a" | "aa" | "ab" | "aba" | "abab" | "abc" | "ac" | "acb" | "acba" | "acbca" |
|--|--|--|--|--|--|--|--|--|--|--|
| [`list< a, b >`](Rule-Reference.md#list-r-s-) | "a" | "a" | "a" | "aba" | "aba" | "a" | "a" | "a" | "a" | "a" |
| [`list_tail< a, b >`](Rule-Reference.md#list_tail-r-s-) | "a" | "a" | "ab" | "aba" | "abab" | "ab" | "a" | "a" | "a" | "a" |
| [`list_must< a, b >`](Rule-Reference.md#list_must-r-s-) | "a" | "a" | **E** | "aba" | **E** | E | "a" | "a" | "a" | "a" |
| [`list< a, b, c >`](Rule-Reference.md#list-r-s-p-) | "a" | "a" | "a" | "aba" | "aba" | "a" | "a" | "a" | "acba" | "acbca" |
| [`list_tail< a, b, c >`](Rule-Reference.md#list_tail-r-s-p-) | "a" | "a" | "ab" | "aba" | "abab" | "ab" | "a" | "acb" | "acba" | "acbca" |
| [`list_must< a, b, c >`](Rule-Reference.md#list_must-r-s-p-) | "a" | "a" | **E** | "aba" | **E** | E | "a" | **E** | "acba" | "acbca" |

The list rules are for non-empty lists, i.e. they fail (locally) when they can not match at least one list element.

## ASCII Matching Rules

|  | "a" | "c" | "e" | "G" | "Z" | "\xA4" |
|--|--|--|--|--|--|--|
| [`any`](Rule-Reference.md#any) | "a" | "c" | "e" | "G" | "Z" | "\xA4" |
| [`one< c, g >`](Rule-Reference.md#one-c-) | **f** | "c" | **f** | **f** | **f** | **f** |
| [`ione< c, g >`](Rule-Reference.md#ione-c-) | **f** | "c" | **f** | "G" | **f** | **f** |
| [`range< c, g >`](Rule-Reference.md#rance-c-d-) | **f** | "c" | "e" | **f** | **f** | **f** |
| [`not_one< c, g >`](Rule-Reference.md#not_one-c-) | "a" | **f** | "e" | "G" | "Z" | "\xA4" |
| [`not_ione< c, g >`](Rule-Reference.md#not_ione-c-) | "a" | **f** | "e" | **f** | "Z" | "\xA4" |
| [`not_range< c, g >`](Rule-Reference.md#not_range-c-d-) | "a" | **f** | **f** | "G" | "Z" | "\xA4" |
| [`any7`](Rule-Reference.md#any7) | "a" | "c" | "e" | "G" | "Z" | **f** |
| [`not_one7< c, g >`](Rule-Reference.md#not_one7-c-) | "a" | **f** | "e" | "G" | "Z" | **f** |
| [`not_ione7< c, g >`](Rule-Reference.md#not_ione7-c-) | "a" | **f** | "e" | **f** | "Z" | **f** |
| [`not_range7< c, g >`](Rule-Reference.md#not_range7-c-d-) | "a" | **f** | **f** | "G" | "Z" | **f** |

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2023-2024 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
