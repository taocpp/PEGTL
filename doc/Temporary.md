# Temporary File

## Failure Modes

The information about how much input is consumed by the rules only applies when the rules succeed.
Otherwise there are two failure modes with different requirements.

- *Local failure* is when a rule returns `false` and the rule **must** generally rewind the input to where its match attempt started.
- *Global failure* is when a rule throws an exception (usually of type `tao::pegtl::parse_error`)(usually via the control-class' `raise()` function).

Since an exception, by default, aborts a parsing run -- hence the term "global failure" -- there are no assumptions or requirements for the throwing rule to rewind the input.

On the other hand a local failure will frequently lead to back-tracking, i.e. the attempt to match a different rule at the same position in the input, wherefore rules that were previously attempted at the same position must rewind back to where they started in preparation of the next attempt.

Note that in some cases it is not necessary to actually rewind on local failure, see the description of the [rewind_mode](Rules-and-Grammars.md#modes) in the section on [how to implement custom rules](Rules-and-Grammars.md#creating-new-rules), and that the PEGTL attempts to minimise superfluous rewinding by statically detecting most of these cases.
