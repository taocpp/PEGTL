# Temporary File

## Failure Modes

The information about how much input is consumed by the rules only applies when the rules succeed.
Otherwise there are two failure modes with different requirements.

- *Local failure* is when a rule returns `false` and the rule **must** generally rewind the input to where its match attempt started.
- *Global failure* is when a rule throws an exception (usually of type `tao::pegtl::parse_error`)(usually via the control-class' `raise()` function).

Since an exception, by default, aborts a parsing run -- hence the term "global failure" -- there are no assumptions or requirements for the throwing rule to rewind the input.

On the other hand a local failure will frequently lead to back-tracking, i.e. the attempt to match a different rule at the same position in the input, wherefore rules that were previously attempted at the same position must rewind back to where they started in preparation of the next attempt.

Note that in some cases it is not necessary to actually rewind on local failure, see the description of the [rewind_mode](Rules-and-Grammars.md#modes) in the section on [how to implement custom rules](Rules-and-Grammars.md#creating-new-rules), and that the PEGTL attempts to minimise superfluous rewinding by statically detecting most of these cases.






Many of these rules can directly be applied to objects in the input, frequently of type `char`, but also to data members or the return values of global or member functions in cases where the input is a sequence of other types.

## Grammar Analysis

Every grammar must be free of cycles that make no progress, i.e. it must not contain unbounded recursive or iterative rules that do not consume any input, as such grammar might enter an infinite loop.
One common pattern for these kinds of problematic grammars is the "dreaded" [left recursion](https://en.wikipedia.org/wiki/Left_recursion) that, while not a problem for less deterministic formalisms like CFGs, must be avoided with PEGs in order to prevent aforementioned infinite loops.

The PEGTL provides a [grammar analysis](Grammar-Analysis.md) that searches the rules of a grammar for cycles that make no progress.
While this could probably be implemented with compile-time meta-programming the implementation in the PEGTL avoids excessive compile times by doing everything at run-time.
It is best practice to create a separate dedicated program that does nothing else but run the grammar analysis, thus keeping this development and debug aid out of the main application.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>

// For this example we use the included JSON grammar.

#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main()
{
   if( pegtl::analyze< grammar >() != 0 ) {
      std::cerr << "Cycles without progress detected!" << std::endl;
      return 1;
   }
   return 0;
}
```
The full example can be found in `src/pegtl/json_analyze.cpp`.
For more information see [Grammar Analysis](Grammar-Analysis.md).

## Tracer

A fundamental tool for grammar development is a *tracer* that prints every step of a parsing run.
It shows exactly which rule was attempted to match where, and what the result was.
The PEGTL provides a tracer that serves both as practical debug tool and as example of the modular nature of the core library that allows for such facilities to be added.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/trace.hpp>

// This example uses the included JSON grammar
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

int main( int argc, char** argv )
{
   if( argc != 2 ) return 1;

   pegtl::argv_input in( argv, 1 );
   pegtl::standard_trace< grammar >( in );

   return 0;
}
```

In the example above, each command line parameter is parsed as a JSON string and a trace is printed to `stderr`.

The full example can be found in `src/pegtl/json_trace.cpp`.
For more information see `tao/pegtl/contrib/trace.hpp`.

## Parse Tree / AST

When developing parsers, a common goal after creating the grammar is to generate a [parse tree](https://en.wikipedia.org/wiki/Parse_tree) or an [AST](https://en.wikipedia.org/wiki/Abstract_syntax_tree).

The PEGTL provides a [Parse Tree](Parse-Tree.md) builder that can filter and/or transform tree nodes on-the-fly.
Additionally, a helper is provided to print out the resulting data structure in [DOT](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format, suitable for creating a graphical representation of the parse tree.

The following example uses a selector to choose which rules generate parse tree nodes, as the graphical representation will usually be too large and confusing when not using a filter and generating nodes for *all* rules.

```c++
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>

// This example uses the included JSON grammar
#include <tao/pegtl/contrib/json.hpp>

namespace pegtl = tao::pegtl;

using grammar = pegtl::must< pegtl::json::text, pegtl::eof >;

template< typename Rule >
using selector = pegtl::parse_tree::selector<
   Rule,
   pegtl::parse_tree::store_content::on<
      pegtl::json::null,
      pegtl::json::true_,
      pegtl::json::false_,
      pegtl::json::number,
      pegtl::json::string,
      pegtl::json::key,
      pegtl::json::array,
      pegtl::json::object,
      pegtl::json::member > >;

int main( int argc, char** argv )
{
   if( argc != 2 ) return 1;

   pegtl::argv_input in( argv, 1 );
   const auto root = parse_tree::parse< grammar, selector >( in );
   if( root ) {
      parse_tree::print_dot( std::cout, *root );
   }

   return 0;
}
```

Running the above program with some example input:

```sh
$ build/src/pegtl/json_parse_tree '{"foo":[true,{}],"bar":[42,null]}' | dot -Tsvg -o json_parse_tree.svg
```

The above will generate an SVG file with a graphical representation of the parse tree.

![JSON Parse Tree](Json-Parse-Tree.svg)

For more information see [Parse Tree](Parse-Tree.md).

## Error Handling

Although the PEGTL could be used without exceptions, most programs will use input classes, grammars and/or actions that can throw exceptions.
Typically, the following pattern helps to print the exceptions in a human friendly way:

```c++
   // The surrounding try/catch for normal exceptions.
   // These might occur if a file can not be opened, etc.
   try {
      tao::pegtl::file_input in( filename );

      // The inner try/catch block, see below...
      try {

         // The actual parser, tracer, parse tree, ...
         pegtl::parse< grammar >( in );

      }
      catch( const pegtl::parse_error& e ) {

         // This catch block needs access to the input
         const auto& p = e.positions_object();
         std::cerr << e.what() << '\n'
                   << tao::pegtl::line_view_at( in, p ) << '\n'
                   << std::setw( p.column ) << '^' << std::endl;

      }
   }
   catch( const std::exception& e ) {

      // Generic catch block for other exceptions
      std::cerr << e.what() << std::endl;

   }
```

For more information see [Errors and Exceptions](Errors-and-Exceptions.md).







# Inputs and Parsing





In the context of PEGTL input classes and positions there is usually an additional (i.e. beyond indicating or supplying the to-be-parsed data) string parameter `source` that identifies where the to-be-parsed data comes from.
For example when parsing a file with one of the appropriate included input classes, the filename is automatically used as `source` so that it will appear in exceptions and error messages.
In other cases the `source` parameter needs to be explicitly passed to the input's constructor.

All classes and functions on this page are in namespace `tao::pegtl`.

## Contents

* [Tracking Mode](#tracking-mode)
* [Line Ending](#line-ending)
* [Source](#source)
* [File Input](#file-input)
* [Memory Input](#memory-input)
* [String Input](#string-input)
* [Stream Inputs](#stream-inputs)
* [Argument Input](#argument-input)
* [Parse Function](#parse-function)
* [Nested Parsing](#nested-parsing)
* [Incremental Input](#incremental-input)
  * [Buffer Size](#buffer-size)
  * [Discard Buffer](#discard-buffer)
  * [Custom Rules](#custom-rules)
  * [Custom Readers](#custom-readers)
  * [Buffer Details](#buffer-details)
* [Error Reporting](#error-reporting)
* [Deduction Guides](#deduction-guides)

## Tracking Mode

Some input classes allow a choice of tracking mode, or whether the `byte`, `line` and `column` counters are continuously updated during a parsing run with `tracking_mode::eager`, or only calculated on-demand in `position()` by scanning the complete input again with `tracking_mode::lazy`.

Lazy tracking is recommended when the position is used very infrequently, for example at most once when a parsing run ends with an exception of type `parse_error`.

Eager tracking is recommended when the position is used frequently and/or in non-exceptional cases, for example when annotating every AST node with the line number.

## Line Ending

All input classes allow the choice of which line endings should be recognised by the `eol` and `eolf` rules, and used for line counting.
The supported line endings are `cr`, a single carriage-return/`"\r"`/`0x0d` character as used on classic Mac OS, `lf`, a single line-feed/`"\n"`/`0x0a` as used on Unix, Linux, Mac OS X and macOS, and `crlf`, a sequence of both as used on MS-DOS and Windows.

The default template parameter for all input classes is `eol::lf_crlf` which recognises both Unix and MS-DOS line endings.
The supplied alternatives are `eol::cr`, `eol::lf`, `eol::crlf` and `eol::cr_crlf`.

## Source

Some input classes allow a choice of how to store the source parameter, with the default being a `std::string`.
When creating many instances of an input class, it can be changed to a non-owning `const char*` to optimise away the memory allocation performed by `std::string`.

## File Input

The classes `file_input<>`, `read_input<>` and, on supported platforms, `mmap_input<>`, can be used to parse the contents of a file.

* `read_input<>` uses C "stdio" facilities to read the file.
* `mmap_input<>` uses `mmap(2)` on POSIX compliant systems or `MapViewOfFile()` on Windows.
* `file_input<>` is derived from `mmap_input<>` when available, and `read_input<>` otherwise, inheriting the respective constructors.

They immediately make available the complete contents of the file; `read_input<>` reads the entire file upon construction.

The constructors that take a `FILE*` argument take ownership of the file pointer, i.e. they `fclose()` it in the destructor.

```c++
template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
struct read_input
{
   explicit read_input( const std::filesystem::path& path );
   read_input( const std::filesystem::path& path, const std::string& source );

   read_input( FILE* file, const std::filesystem::path& path );
   read_input( FILE* file, const std::filesystem::path& path, const std::string& source );
};

template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
struct mmap_input
{
   explicit mmap_input( const std::filesystem::path& path );
   mmap_input( const std::filesystem::path& path, const std::string& source );
};

template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
using file_input = mmap_input< P, Eol >;  // Or read_input when no mmap_input available.
```

## Memory Input

The class `memory_input<>` can be used to parse existing contiguous blocks of memory like the contents of a `std::string`.
The input **neither copies the data nor takes ownership, it only keeps pointers**.
The various constructors accept the to-be-parsed data in different formats.
The `source` parameter is required for all constructors to disambiguate the different overloads.
If you don't want to specify a source just use the empty string (`""`).

The constructors that only takes a `const char* begin` for the data uses `std::strlen()` to determine the length.
It will therefore *only* work correctly with data that is terminated with a 0-byte (and does not contain embedded 0-bytes, which are otherwise fine).

The constructors that take additional `byte`, `line` and `column` arguments initialise the internal counters with the supplied values, rather than the defaults of `0`, `1` and `1`.

```c++
template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf, typename Source = std::string >
class memory_input
{
   template< typename T >
   memory_input( const internal::iterator& iter, const char* end, T&& source ) noexcept(...);

   template< typename T >
   memory_input( const char* begin, const char* end, T&& source ) noexcept(...);

   template< typename T >
   memory_input( const char* begin, const std::size_t size, T&& source ) noexcept(...);

   template< typename T >
   memory_input( const std::string& string, T&& source ) noexcept(...);

   template< typename T >
   memory_input( const char* begin, T&& source ) noexcept(...);

   template< typename T >
   memory_input( const char* begin, const char* end, T&& source,
                 const std::size_t byte, const std::size_t line, const std::size_t column ) noexcept(...);
};
```

### Examples

###### Example 1

```c++
memory_input in1( "this is the input to parse", "" );
```

Construct a `memory_input` with default tracking mode, default end-of-line mode (accepting Unix and MS-DOS line endings), and default source storage.
As there are only two parameters, the 5th overload from above is choosen.
The data to parse is given directly as a string literal which is not copied.
As no pointer to the end or the size of the input is given, the length of the data to be parsed will be determined by calling `strlen` on the pointer passed as the first parameter.
The source is the empty string.

###### Example 2

```c++
struct packet
{
   // ...
   const std::array< char >& buffer() const noexcept;
   std::string identifier() const;
   // ...
};

packet p = ...; // some UDP packet class

memory_input< tracking_mode::lazy, eol::crlf > in2( p.buffer().begin(), p.buffer().end(), p.identifier() );
```

Consider a UDP packet that was received and should be parsed.
Construct a `memory_input` with lazy tracking mode, MS-DOS end-of-line mode (accepting only MS-DOS line endings), and default source storage.
This example chooses the second overload from above.
The data to parse is given as two `const char*` pointers (as the data is not null-terminated) and is, of course, not copied.
Consider the source to be an identifier for the packet that was received, e.g. a string constructed from the timestamp, the source IP/port, the interface it was received on, a sequence number, or similar information.
Note that this example shows why the source parameter is necessary to disambiguate the overloads.
If the source would be optional (defaulted), the signature of this overload would also match the first example and therefore be ambiguous.

### Additional Remarks

Note that `noexcept(...)` is a conditional noexcept-specification, depending on whether the construction of the source stored in the class can throw given the perfectly-forwarded parameter `source`. Technically, it is implemented as `noexcept( std::is_nothrow_constructible< Source, T&& >::value )`.

With the default `Source` type of `std::string`, the `source` parameter to the constructors is usually a `const char*` or (any reference to) a `std::string`, but anything that can be used to construct a `std::string` will work. When `Source` is set to `const char*` then only a `const char *` (or something that can implicitly be converted to one) will work.

The implementation of the constructors is different than shown.
They should be used "as if" this was the actual signature.

## String Input

The class `string_input<>` can also be used to parse a `std::string`.
Unlike class `memory_input<>`, this class stores a copied (or moved) version of the data for which it takes ownership.

```c++
template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf, typename Source = std::string >
class string_input
{
   template< typename V, typename T >
   string_input( V&& data, T&& source ) noexcept(...);

   template< typename V, typename T >
   string_input( V&& data, T&& source,
                 const std::size_t byte, const std::size_t line, const std::size_t column ) noexcept(...);
};
```

### Example

```c++
std::string content(); // returns the content

string_input in1( content(), "from_content" );
```

Construct a `string_input` with default tracking mode, default end-of-line mode (accepting Unix and MS-DOS line endings), and default source storage.
The data returned from calling `content()` is copied into the input.
The source is `from_content`.

### Additional Remarks

Note that the implementation of the constructors is different than shown.
They should be used "as if" this was the actual signature.

## Stream Inputs

The classes `cstream_input<>` and `istream_input<>` can be used to parse data from C-streams (`std::FILE*`) and C++-streams (`std::istream`), respectively.
Unlike the file inputs above, they internally use `buffer_input<>` and therefore do *not* read the complete stream upon construction.

They all have a single constructor that takes a stream, the maximum buffer size, and the name of the source.
Note that these classes only keep a pointer/reference to the stream and do **not** take ownership; in particular `cstream_input<>` does **not** call `std::close()`.

See [Incremental Input](#incremental-input) for details on the `maximum` argument, and how to use the mandatory [discard facilities](#discard-buffer).

```c++
template< typename Eol = eol::lf_crlf >
struct cstream_input
{
   cstream_input( std::FILE* stream, const std::size_t maximum, const char* source );
   cstream_input( std::FILE* stream, const std::size_t maximum, const std::string& source );
};

template< typename Eol = eol::lf_crlf >
struct istream_input
{
   istream_input( std::istream& stream, const std::size_t maximum, const char* source );
   istream_input( std::istream& stream, const std::size_t maximum, const std::string& source );
};
```

Note that the implementation of the constructors is different than shown.
They should be used "as if" this was the actual signature.

## Argument Input

The class `argv_input<>` can be used to parse a string passed from the command line.

```c++
template< tracking_mode P = tracking_mode::eager, typename Eol = eol::lf_crlf >
class argv_input
{
   argv_input( char** argv, const std::size_t n );
   argv_input( char** argv, const std::size_t n, const char* source );
   argv_input( char** argv, const std::size_t n, const std::string& source );
};
```

If no `source` is given, the source is set to `"argv[N]"` where N is the string representation of `n`.

Note that the implementation of the constructors is different than shown.
They should be used "as if" this was the actual signature.

## Parse Function

The parse functions accept the following template parameters and arguments:

- The [`Rule` class](Rules-and-Grammars.md) represents the top-level parsing rule of the grammar and is mandatory.
- The [`Action<>` class template](Actions-and-States.md) is required to actually do something during a parsing run.
- The [`Control<>` class template](Control-and-Debug.md) is only required for grammar debugging or some advanced uses.
- The [`States`](Actions-and-States.md#changing-states) are the types of the objects that are passed to all actions and control hooks.

Additionally, two enumeration values can be used to control the behaviour:

- The `apply_mode`, which can also be set to `nothing` in order to disable action invocations, just like the `disable<>` rule does.
- The `rewind_mode`, which can also be set to `required` when rewinding the input to its start is required for top-level parse failures.

The result of a parsing run, i.e. an invocation of `tao::pegtl::parse()`, can be either

- *success*, a return value of `true`,
- *local failure*, a return value of `false`,
- *global failure*, an exception of type `tao::pegtl::parse_error`, or
- any other exception thrown by the input class or an action function.

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::action,
          rewind_mode M = rewind_mode::dontcare,
          typename ParseInput,
          typename... States >
bool parse( ParseInput& in,
            States&&... st );
```

## Nested Parsing

Nested parsing refers to an (inner) parsing run that is performed "in the middle of" another (outer) parsing run, for example when one file "includes" another file.

The difference to the regular `tao::pegtl::parse()` function is that when a global failure occurs within `tao::pegtl::parse_nested()` then a new exception is thrown via `Control< Rule >::raise_nested()`.
The new exception contains the previous one as nested exception. The functions in the header `tao/pegtl/contrib/nested_exceptions.hpp` can be used to work with these nested exceptions.
The inner-most exception that was thrown first will be the "most nested" exception, i.e. the final one in the linked list of nested exceptions.

The position information contained in the nested exceptions allows for error messages like "error in file F1 line L1 included from file F2 line L2 etc."

Calling `parse_nested()` requires the input from the outer parsing run, or the position whithin the outer parsing run, as additional first argument ("additional" as compared to `parse()`).

```c++
template< typename Rule,
          template< typename... > class Action = nothing,
          template< typename... > class Control = normal,
          apply_mode A = apply_mode::action,
          rewind_mode M = rewind_mode::dontcare,
          typename OuterInput,  // Can also be class position.
          typename ParseInput,
          typename... States >
bool parse_nested( const OuterInput& oi,
                   ParseInput& in,
                   States&&... st );
```

## Incremental Input

The PEGTL is designed and optimised for parsing single contiguous blocks of memory like a memory-mapped file or the contents of a `std::string`.
In cases where the data does not fit into memory, or other reasons prevent parsing the data as single memory block, an *incremental* input can be used.

This allows parsing with only (small) portions of the input in a memory buffer at any single time.
The buffer is filled automatically, however the [*discard* facilities](#discard-buffer) must be used to regularly flush the buffer and make space for a new portion of input data.

The [stream inputs](#stream-inputs) are ready-to-use input classes for C++-style and C-style streams.
Apart from having to use the [discard facilities](#discard-buffer), and some extra care when implementing [custom rules](#custom-rules), they can be used just like any other [input class](Inputs-and-Parsing.md).

### Buffer Size

The [stream inputs](#stream-inputs), and all other inputs based on `buffer_input<>`, contain a buffer that is allocated in the constructor.
The buffer capacity is the sum of a *maximum* value and a *chunk* size.

The maximum value is passed to the constructor as function argument, the chunk size is a (rarely changed) template parameter.
The required buffer capacity depends on the grammar, the actions, *and* the input data.

The buffer must be able to hold

* any and all data for look-ahead performed by the grammar,
* any and all data for back-tracking performed by the grammar,
* any and all data for actions' [`apply()`](Actions-and-States.md#apply) (not [`apply0()`](Actions-and-States.md#apply0)).

For example consider an excerpt from the JSON grammar from `include/tao/pegtl/contrib/json.hpp`.

```c++
struct xdigit : abnf::HEXDIG {};
struct unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
struct escaped_char : one< '"', '\\', '/', 'b', 'f', 'n', 'r', 't' > {};
struct escaped : sor< escaped_char, unicode > {};
struct unescaped : utf8::range< 0x20, 0x10FFFF > {};
struct char_ : if_then_else< one< '\\' >, must< escaped >, unescaped > {};

struct string_content : until< at< one< '"' > >, must< char_ > > {};
struct string : seq< one< '"' >, must< string_content >, any >
{
   using content = string_content;
};
```

The rule `string_content` matches JSON strings as they occur in a JSON document.
If an action with `apply()` (rather than `apply0()`) is attached to the `string_content` rule, the buffer capacity is an upper bound on the length of the JSON strings that can be processed.

If the actions are only attached to say `unescaped`, `escaped_char` and `rep< 4, must< xdigit > >`, the latter because it, too, occurs in an (implicit, inside of `list`) unbounded loop, then the JSON strings are processed unescaped-character-by-unescaped-character and escape-sequence-by-escape-sequence.
As long as the buffer is [discarded](#discard-buffer) frequently, like after every unescaped character and every single escape sequence, a buffer capacity as small as 8 or 12 should suffice for parsing arbitrarily long JSON strings.

Note that the [`eof`](Rule-Reference.md#eof) rule requires at least one byte of free buffer space when there is no unconsumed data in the buffer.

### Discard Buffer

To prevent the buffer from overflowing, the `discard()` member function of class `buffer_input<>` must be called regularly.

**Discarding invalidates all pointers to the input's data and MUST NOT be used where backtracking to before the discard might occur AND/OR nested within a rule for which an action with input can be called.**

Calling `discard()` on a non-buffered input is an empty method and will be optimised away completely.

Usually you don't call `discard()` manually. Instead, one of the two following methods might be used.

#### Via Rules

The [`discard`](Rule-Reference#discard) rule behaves just like the [`success`](Rule-Reference.md#success) rule but calls the discard function on the input before returning `true`.

#### Via Actions

The `tao::pegtl::discard_input`, `tao::pegtl::discard_input_on_success` and `tao::pegtl::discard_input_on_failure` [actions](Actions-and-States.md) can be used to discard input non-intrusively, i.e. without changing the grammar like with the [`discard`](Rule-Reference.md#discard) rule.

These actions are used in the usual way, by deriving a custom action class template specialisation from them.
In the case of `discard_input`, the input is discarded unconditionally after every match attempt of the rule that the action is attached to.
As `discard_input` is based on the `match()` method, it is unaffected by enabling or disabling actions (which only applies to the `apply`/`apply0`-methods).

The other two variants behave as implied by their respective names, keeping in mind that "failure" is to be understood as "local failure" (false), no discard is performed on global failure (exception).
Similarly "unconditional" is wrt. success or local failure, not global failure.

```c++
template<>
struct my_action< R >
   : tao::pegtl::discard_input
{
   // It is safe to implement apply() here if appropriate:
   // discard() will be called by discard_input's match()
   // only _after_ calling this action's apply().
};
```

In practice, since the "must"-rules like `must<>` and `if_must<>` inhibit backtracking, they can be good indicators of where to perform a discard.
For example consider again this rule from the JSON grammar from `include/tao/pegtl/contrib/json.hpp`.

```c++
struct unicode : list< seq< one< 'u' >, rep< 4, must< xdigit > > >, one< '\\' > > {};
```

The `xdigit` rule is within a `must`, wherefore we know that no backtracking is possible, so we could discard after `xdigit` or `must< xdigit >`.
However then we can't attach an action with [`apply()`](Actions-and-States.md#apply) to the `rep< 4, ... >` since we would be discarding after every single digit.
This is not ideal, it would be more efficient to process all four xdigits in a single action invocation.

Looking close we can see that backtracking to before the `rep<>` is actually impossible because once the `list<>` has successfully matched `seq< one< 'u' >, rep< 4, must< xdigit > > >` it will never go back.
It will attempt to match another backslash, the list item separator, and if successful loop to the `seq<>`, but once the next character is a `'u'`, the `must<>` in the `rep` seals the deal, there is no way to not complete the next list entry.

Therefore we can safely attach an action to the `rep<>` that processes the four xdigits and then discards the input.

```c++
template<>
struct my_action< rep< 4, must< xdigit > >
   : tao::pegtl::discard_input
{
   template< typename ActionInput >
   static void apply( const ActionInput& in, /* the states */ )
   {
      assert( in.size() == 4 );
      // process the 4 xdigits
   }
};
```

Another good candidate in the JSON grammar to discard after is the `tao::pegtl::json::value` rule...

### Custom Rules

All incremental inputs included with the library and documented here are based on `buffer_input<>`.
A custom rule that is compatible with incremental inputs needs to pay attention to the `amount` argument in the input's interface.
Unlike the inputs based on `memory_input<>`, the `size( amount )` and `end( amount )` member functions do not ignore the `amount` argument, and the `require( amount )` member function is not a complete dummy.

```c++
template< ... >
class buffer_input
{
   bool empty();
   std::size_t size( const std::size_t amount );
   const char* end( const std::size_t amount );
   void require( const std::size_t amount );
   ...
};
```

The `require( amount )` member function tells the input to make available at least `amount` unconsumed bytes of input data.
It is not normally called directly unless there is good reason to prefetch some data.

The `empty()`, `size( amount )` and `end( amount )` member functions call `require( amount )`, or, in the case of `empty()`, `require( 1 )`.
The `amount` parameter should be understood as a parsing rule wishing to inspect and consume *up to* `amount` bytes of input.

A custom rule must make sure to use appropriate values of `amount`.
For examples of how the `amount` is set by parsing rules please search for `in.size` in `include/tao/pegtl/internal/`.

### Custom Readers

An incremental input consists of `buffer_input<>` together with a *reader*, a class or function that is used by the buffer input to fill the buffer.

The buffer input is a class template with multiple template parameters.

```c++
template< typename Reader,
          typename Eol = eol::lf_crlf,
          typename Source = std::string,
          std::size_t Chunk = 64 >
class buffer_input;
```

The `Eol` and `Source` parameters are like for the other [input classes](Inputs-and-Parsing.md#memory-input).
The `Chunk` parameter is explained below in detail.
The `Reader` can be anything that can be called like the following wrapper.

```c++
std::function< std::size_t( char* buffer, const std::size_t length ) >
```

The arguments and return value are similar to other `read()`-style functions, a request to read `length` bytes into the memory pointed to by `buffer` that returns the number of bytes actually read.
Reaching the end of the input MUST be the only reason for the reader to return zero.
The reader might be called again after returning zero, with the expectation of returning zero again.

Note that `buffer_input<>` consumes the first two arguments to its constructor for the *source* and *maximum*, and uses perfect forwarding to pass everything else to the constructor of the embedded instance of `Reader`.

For examples of how to implement readers please look at `istream_reader.hpp` and `cstream_reader.hpp` in `include/tao/pegtl/internal/`.

### Buffer Details

The buffer input's `Chunk` template parameter is actually used in multiple places.

1. The `maximum` buffer capacity passed by the user is incremented by `Chunk`.
2. A discard does nothing when there are less than `Chunk` bytes of consumed buffered data.
3. The buffer input requests at least `Chunk` bytes from the reader if there is enough space.

Note that the first and second point go hand-in-hand, in order to optimise away some discards, the buffer must be extended in order to guarantee that at least `maximum` bytes can be buffered after a call to discard, even when it does nothing. The third point is simply an optimisation to call the reader less frequently.

## Error Reporting

When reporting an error, one often wants to print the complete line from the input where the error occurred and a marker at the position where the error is found within that line.
To support this, the `memory_input<>` class has a member function `at( p )` that returns a `const char*` to the input byte corresponding to `p` of type `tao::pegtl::position`.
This function is the basis for the following convenience functions thar are _not_ member functions of the input.

The functions `begin_of_line( in, p )` and `end_of_line_or_file( in, p )` return a `const char*` to the begin-of-line before `p`, the end-of-line (or file) after `p`, respectively.
The function `line_view_at( in, p )` combines the former two and returns a `std::string_view` to the line surrounding the position indicated by `p`.
As usual these function reside in namespace `TAO_PEGTL_NAMESPACE` which defaults to `tao::pegtl`.
Example usage:

```c++
// create input 'in' here...
try {
  // call parse on the input 'in' here...
}
catch( const parse_error& e ) {
   const auto& p = e.position_object();
   std::cerr << e.what() << std::endl
             << line_view_at( in, p ) << '\n'
             << std::setw( p.column ) << '^' << std::endl;
}
```

All input classes based on `memory_input<>` support the above, while all classes based on `buffer_input<>` are unable to supply the same functionality as previous input might have been discarded already.
Trying to call any of those functions on `buffer_input<>`-based instances will lead to a compile error.

## Deduction Guides

All input classes support [deduction guides](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction), e.g. instead of `file_input<> in( "filename.txt" )` one can use `file_input in( "filename.txt" )`.

---

This document is part of the [PEGTL](https://github.com/taocpp/PEGTL).

Copyright (c) 2014-2025 Dr. Colin Hirsch and Daniel Frey<br>
Distributed under the Boost Software License, Version 1.0<br>
See accompanying file [LICENSE_1_0.txt](../LICENSE_1_0.txt) or copy at https://www.boost.org/LICENSE_1_0.txt
