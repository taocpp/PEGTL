# 2014 Refactoring

In 2014 (and well into 2015) the 0.x versions of the PEGTL were significantly refactored.
The important changes are listed below.

- Deprecated old site on Google code and published new version on GitHub.
- Removed the semi-automatic pretty-printing of grammar rules; now the class names are used, when possible demangled.
- Renamed some of the classes that have more words in their name to use an underscore, e.g. `ifmust<>` is now `if_must<>`.
- The input layer was simplified and can only parse memory blocks and files which allows for some optimisations, with possible future work in this area to find a more generic solution if necessary.
- Removed the rules `apply<>` and `if_apply<>` that were used to directly call actions from within the grammar, and:
- Where the other method of attaching actions to rules in PEGTL 0.x required specialisation of a given class template `action<>`, in PEGTL 1.y the action class template can be chosen by the user and changed at any point in the grammar.
- As a side-effect there is a much cleaner way of enabling and disabling actions in a portion of the grammar.
- Actions now have access to the current position in the input, i.e. to the filename, and line and column number.
- Actions now receive a pointer to, and the size of, the matched portion of the input (previously a `std::string` with a copy of the matched data), therefore:
- There is no distinction between actions that require access to the matched data and those that don't, furthermore:
- The object via which the actions gain access to the matched data is of the same type that the rules work on, so actions can easily invoke another grammar on the matched data.
- The `at<>` and `not_at<>` rules now call their subordinate rules with actions disabled.
- The variadic `states...` arguments that are passed through all rule invocations for use by the actions are *not* forwarded with `std::forward<>` anymore since it (usually) doesn't make much sense to move them, and accidentially moving multiple times was a possible error scenario.
- There are now five different `rep` rules for repeating a sequence of rules with more control over the acceptable or required number of repetitions.
- There are new rules `try_catch<>` and `try_catch_type<>` that convert global errors, i.e. exceptions, into local errors, i.e. a return value of `false`.
- Unified concept for actions and debug hooks, i.e. just like the actions are called from a class template that is passed into the top-level `parse()`-function, there is another class template that is called for debug/trace and error throwing purposes; both can be changed at any point within the grammar.
- A large under-the-hood reorganisation has the benefit of preventing actions from being invoked on rules that are implementation details of other rules, e.g. the `pad< Rule, Padding >` rule contains `star< Padding >` in its implementation, so a specialisation of the action-class-template for `star< Padding >` would be called within `pad<>`, even though the `star< Pad >` was not explicitly written by the user; in PEGTL 1.y these unintended action invocations no longer occur.
- Partial support for Unicode has been added in the form of some basic rules like `one<>` and `range<>` also being supplied in a UTF-8 (and experimental UCS-4) aware version(s) that can correctly process arbitrary code points from `0` to `0x10ffff`.
- The supplied input class works together with the supplied exception throwing to support better error locations when performing nested file parsing, i.e. a `parse_error` contains a vector of parse positions.
- Added a function to analyse a grammar for the presence of infinite loops, i.e. cycles in the rules that do not (necessarily) consume any input like left recursion.
- As actions are applied to a grammar in a non-invasive way, several common grammars were added to the PEGTL as documented in [Contrib and Examples](Contrib-and-Examples.md).
- The `list<>`-rule was replaced by a set of new list rules with different padding semantics.
- The `at_one<>` and other rules `foo` that are merely shortcuts for `at< foo >` were removed.
- The `if_then<>` rule was removed.
- The `error_mode` flag was removed.
- The semantics of the `must<>` rules was changed to convert local failure to global failure only for the immediate sub-rules of a `must<>` rule.
- The `parse` methods no longer implicitly generate a global failure, they can return a local failure as a Boolean value. If a global failure is required, the top-level grammar rule has to start with a `must<>`.

Given the possible complexity of PEGs the relatively straightforward infinite loop analysis will not always give the correct result.
It has however proven useful in finding issues with grammars converted from other, more descriptive (rather than imperative) forms where such infinite loops are not an issue.

Copyright (c) 2014-2016 Dr. Colin Hirsch and Daniel Frey
