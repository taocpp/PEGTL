# Errors and Exceptions

A parsing run, a call to one of the `parse()`-functions as explained in [Inputs and Parsing](Inputs-and-Parsing.md), can have the same results as calling the `match()`-method on a grammar rule.

* A return value of `true` indicates a *succesful* match.
* A return value of `false` is called a *local failure* (even when propagated to the top).
* An exception indicating a *global failure* is thrown.

The PEGTL parsing rules throw exceptions of type `tao::pegtl::parse_error`, some of the inputs throw exceptions of type `tao::pegtl::input_error`.
Other exception classes can be used freely from actions and custom parsing rules.

## Contents

* [Local to Global Failure](#local-to-global-failure)
* [Global to Local Failure](#global-to-local-failure)
* [Examples for Must Rules](#examples-for-must-rules)
* [Custom Exception Messages](#custom-exception-messages)

## Local to Global Failure

A local failure returned by a parsing rule is not necessarily propagated to the top, for example when the rule is

1) in a rule like `at<>`, `opt<>` or `star<>`, or
2) not the last rule inside an `sor<>` combinator.

To convert local failures to global failures, the `must<>` combinator rule can be used (together with related rules like `if_must<>`, `if_must_else<>` and `star_must<>`).
The `must<>` rule is equivalent to `seq<>` in that it attempts to match all sub-rules in sequence, but converts all local failures of the (direct) sub-rules to global failures.

Global failures can also be unconditionally provoked with the `raise<>` grammar rule which is more flexible since the template argument can be any type, not just a parsing rule.
It should be mentioned that `must< R >` is semantically equivalent to `sor< R, raise< R > >`, but more efficient.

In any case, the task of actually throwing an exception is delegated to the [control class'](Control-and-Debug.md) `raise()`-method.

## Global to Local Failure

To convert global failure to local failure, the grammar rules [`try_catch`](Rule-Reference.md#try_catch-r-) and [`try_catch_type`](Rule-Reference.md#try_catch_type-e-r-) can be used.
Since these rules are not very commonplace they are ignored in this document, in other words we assume that global failure always propagages to the top.

## Examples for Must Rules

One basic use case of the `must<>` rule is as top-level grammar rule.
Then a parsing run can only be successful or throw an exception, it is not necessary to check the return value of the `parse()` function.

For another use case consider the following parsing rules for an simplified C-string literal that only allows `\n`, `\r` and `\t` as escape sequences.
The rule `escaped` is for a single escaped characters, the rule `content` is for the complete content of such a literal.

```c++
   using namespace tao::pegtl;
   struct escaped : seq< one< '\\' >, one< 'n', 'r', 't' > > {};
   struct content : star< sor< escaped, not_one< '\\' > > > {};
   struct literal : seq< one< '"' >, content, one< '"' > > {};
```

The `escaped` rule first matches a backslash, and then one of the allowed subsequent characters.
When either of the two `one<>` rules returns a local failure, then so will `escaped` itself.
In that case backtracking is performed, and the `sor<>` rule attempts to match the `not_one< '\\' >` at the same position instead.

This backtracking is appropriate if the `escaped` rule failed to match for lack of a backslash in the input.
It is however inappropriate when the backslash was not followed by one of the allowed characters since we know that there is no other possibility in the grammar that will lead to a successful match.

We can threfore re-write the `escaped` rule as follows to express that once the backslash has matched we need one of the following allowed characters to match, otherwise a global failure is thrown.

```c++
   using namespace tao::pegtl;
   struct escaped : seq< one< '\\' >, must< one< 'n', 'r', 't' > > > {};
```

An `seq<>` where all but the first sub-rule is inside a `must<>` occurs frequently enough to merit a convenience rule.
The following rule is equivalent to the above.

```c++
   using namespace tao::pegtl;
   struct escaped : if_must< one< '\\' >, one< 'n', 'r', 't' > > {};
```

Now the `escaped` rule can only return local failure when the next input byte is not a backslash.
This knowledge can be used to simplify the `content` rule by not needing to exclude it in the following rule.

```c++
   using namespace tao::pegtl;
   struct content : star< sor< escaped, any > > {};
```

The same use of `if_must<>` can be applied to the `literal` rule assuming that it occurs in some `sor<>` where it is the only rule whose matched input can begin with a quotation mark...

Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
