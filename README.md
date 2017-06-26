# C Left-Right Parser (libcleri)
Language parser for the C program language. Initially created for [SiriDB](https://github.com/transceptor-technology/siridb-server).

---------------------------------------
  * [Installation](#installation)
  * [Related projects](#related-projects)
  * [Quick usage](#quick-usage)
  * [API](#api)
    * [cleri_t](#cleri_t)
    * [cleri_grammar_t](#cleri_grammar_t)
    * [cleri_parse_t](#cleri_parse_t)
    * [cleri_node_t](#cleri_node_t)
    * [cleri_children_t](#cleri_children_t)
    * [cleri_olist_t](#cleri_olist_t)
  * [Elements](#elements)
    * [cleri_keyword_t](#cleri_keyword_t)
    * [cleri_regex_t](#cleri_regex_t)
    * [cleri_choice_t](#cleri_choice_t)
    * [cleri_sequence_t](#cleri_sequence_t)
    * [cleri_optional_t](#cleri_optional_t)
    * [cleri_prio_t](#cleri_prio_t)
    * [cleri_repeat_t](#cleri_repeat_t)
    * [cleri_list_t](#cleri_list_t)
    * [cleri_token_t](#cleri_repeat_t)
    * [cleri_tokens_t](#cleri_tokens_t)
    * [Forward reference](#forward-reference)
    * [cleri_dup_t](#cleri_dup_t)
  * [Miscellaneous functions](#miscellaneous-functions)

---------------------------------------

## Installation
>Note: libcleri requires [pcre](http://www.pcre.org/)
>
>On Ubuntu:
>
>`sudo apt install libpcre3-dev`
>
>On MacOs:
>
>`brew install pcre`
>
Install debug or release version, in this example we will install the release version.
```
$ cd Release
```

Compile libcleri
>Note: On MacOs you might need to set environment variables:
>
>`export CFLAGS="-I/usr/local/include" && export LDFLAGS="-L/usr/local/lib"`
>
```
$ make all
```

Install libcleri
```
$ sudo make install
```

> Note: run `sudo make uninstall` for removal.

## Related projects
- [pyleri](https://github.com/transceptor-technology/pyleri): Python parser (can export grammar to pyleri, libcleri, goleri and jsleri)
- [jsleri](https://github.com/transceptor-technology/jsleri): JavaScript parser
- [goleri](https://github.com/transceptor-technology/goleri): Go parser

## Quick usage
>The recommended way to create a grammar is to use [pyleri](https://github.com/transceptor-technology/pyleri) for
>writing the grammar and then export the grammar to libcleri or other languages.

This is a simple example using libcleri:
```c
#include <stdio.h>
#include <cleri/cleri.h>

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_parse_free(pr);
}

int main(void)
{
    /* define grammar */
    cleri_t * k_hi = cleri_keyword(0, "hi", 0);
    cleri_t * r_name = cleri_regex(0, "^(?:\"(?:[^\"]*)\")+");
    cleri_t * start = cleri_sequence(0, 2, k_hi, r_name);

    /* compile grammar */
    cleri_grammar_t * my_grammar = cleri_grammar(start, NULL);

    /* test some strings */
    test_str(my_grammar, "hi \"Iris\"");  // true
    test_str(my_grammar, "bye \"Iris\""); // false

    /* cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}
```

## API

### `cleri_t`
Cleri type is the base object for each element.

*Public members*
- `uint32_t gid`: Global Identifier for the element. This GID is not required and
as a rule it should be set to 0 if not used. You can use the GID for identifiying
an element in a parse result. When exporting a Pyleri grammar, each named element
will have set the GID be default. (readonly)
- `cleri_tp tp`: Type for the cleri object. (readonly)
    - `CLERI_TP_SEQUENCE`
    - `CLERI_TP_OPTIONAL`
    - `CLERI_TP_CHOICE`
    - `CLERI_TP_LIST`
    - `CLERI_TP_REPEAT`
    - `CLERI_TP_PRIO`
    - `CLERI_TP_RULE`
    - `CLERI_TP_THIS`
    - `CLERI_TP_KEYWORD`
    - `CLERI_TP_TOKEN`
    - `CLERI_TP_TOKENS`
    - `CLERI_TP_REGEX`
    - `CLERI_TP_END_OF_STATEMENT`
- `cleri_via_t via`: Element. (readonly)
    - `cleri_sequence_t * sequence`
    - `cleri_optional_t * optional`
    - `cleri_choice_t * choice`
    - `cleri_list_t * list`
    - `cleri_repeat_t * repeat`
    - `cleri_prio_t * prio`
    - `cleri_rule_t * rule`
    - `cleri_keyword_t * keyword`
    - `cleri_regex_t * regex`
    - `cleri_token_t * token`
    - `cleri_tokens_t * tokens`
    - `void * dummy` (place holder, this, eof)

#### `cleri_t * cleri_new(uint32_t gid, cleri_tp tp, cleri_free_object_t free_object, cleri_parse_object_t parse_object)`
Create and return a new cleri object. A unique gid is not required but can help
you with identifiying the element in a [parse result](#cleri_parse_t). As a rule
you should assign 0 in case no specific gid is required. This function should only
be used in case you want to create your own custom element.

#### `void cleri_incref(cleri_t * cl_object)`
Increment the reference counter for a cleri object. Should only be used in case you
want to write your own custom element.

#### `void cleri_decref(cleri_t * cl_object)`
Decrement the reference counter for a cleri object. If no references are left the
object will be destroyed. Do not use this function after the element has
successfully been added to another element or grammar. Should only be used in
case you want to write your own custom element.

#### `int cleri_free(cleri_t * cl_object)`
Decrement reference counter for a cleri object. When there are no more references
left the object will be destroyed. Use this function to cleanup after errors
have occurred. Do not use this function after the element has successfully been
added to another element or grammar.

Example strict error handling:
```c
cleri_grammar_t * compile_grammar(void)
{
    cleri_t * k_hello = cleri_keyword(0, "hello", 0);
    if (k_hello == NULL) {
        return NULL;
    }
    cleri_t * k_world = cleri_keyword(0, "world", 0);
    if (k_world == NULL) {
        cleri_free(k_hello); // must cleanup k_hello
        return NULL;
    }
    cleri_t * hello_world = cleri_sequence(0, 2, k_hello, k_world);
    if (start == NULL) {
        cleri_free(k_hello);
        cleri_free(k_world);
        return NULL;
    }
    cleri_t * opt = cleri_optional(0, hello_world);
    if (opt == NULL) {
        /* we now must only cleanup hello_world since this sequence will
        * cleanup both keywords too. */
        cleri_free(hello_world);
        return NULL;
    }
    cleri_grammar_t * grammar = cleri_grammar(opt, NULL);
    if (grammar == NULL) {
        cleri_free(opt);
    }
    /* when your program has finished, the grammar including all elements can
     * be destroyed using cleri_grammar_free() */
    return grammar;
}
```
>Note: Usually grammar is only compiled at the startup of your program so
>memory allocation errors during the grammar creation are unlikely to occur.
>If NULL is parsed as an argument instead of an element, then the function
>to which the argument is parsed to, will return NULL. Following this
>chain the final grammar returns NULL in case an error has occurred somewhere.
>In this case you should usually abort the program.

### `cleri_grammar_t`
Compiled libcleri grammar.

*No public members*

#### `cleri_grammar_t * cleri_grammar(cleri_t * start, const char * re_keywords)`
Create and return a compiled grammar. Argument `start` must be the entry element
for the grammar. Argument `re_keywords` should be a regular expression starting
with character `^` for matching keywords in a grammar. When a grammar is created,
each defined [keyword](#cleri_keyword_t) should match this regular expression.
`re_keywords` is allowed to be `NULL` in which case the defualt
`CLERI_DEFAULT_RE_KEYWORDS` is used.

#### `void cleri_grammar_free(cleri_grammar_t * grammar)`
Cleanup grammar. This will also destroy all elements which are used by the
grammar. Make sure all parse results are destroyed before destroying the grammar
because a [parse result](#cleri_parse_t) depends on elements from the grammar.

### `cleri_parse_t`
Parse result containing the parse tree and other information about the parse
result.

*Public members*
- `int cleri_parse_t.is_valid`: Boolean. Value is 1 (TRUE) in case the parse string is valid or 0 (FALSE) if not. (readonly)
- `size_t cleri_parse_t.pos`: Position in the string to where the string was successfully parsed. This value is (readonly)
equal to the length of the string in case `cleri_parse_t.is_valid` is TRUE. (readonly)
- `const char * cleri_parse_t.str`: Pointer to the provided string. (readonly)
- `cleri_node_t * tree`: Parse tree. (see [cleri_node_t](#cleri_node_t) and [cleri_children_t](#cleri_children_t)) (readonly)
- `const cleri_olist_t * expect`: Linked list to possible elements at position `cleri_parse_t.pos` in `cleri_parse_t.str`.
(see [cleri_olist_t](#cleri_olist_t) for more information)

#### `cleri_parse_t * cleri_parse(cleri_grammar_t * grammar, const char * str)`
Create and return a parse result. The parse result contains pointers to the
provided string (`str`) so make sure the string is available while using the
parse result.

#### `void cleri_parse_free(cleri_parse_t * pr)`
Cleanup a parse result.

#### `void cleri_parse_expect_start(cleri_parse_t * pr)`
Can be used to reset the expect list to start. Usually you are not required to
use this function since the expect list is already at the start position.

### `cleri_node_t`
Node object. A parse result has a parse tree which consists of nodes. Each node
may have children.

*Public members*
- `const char * cleri_node_t.str`: Pointer to the position in the parse string where this node starts. (readonly)
- `size_t cleri_node_t.len`: Length of the string which is applicable for this node. (readonly)
- `cleri_t * cleri_node_t.cl_obj`: Element from the grammar which matches this node. (readonly)
- `cleri_children_t * cleri_node_t.children`: Optional children for this node. (readonly)

#### `bool cleri_node_has_children(cleri_node_t * node)`
Macro function for checking if a node has children.

### `cleri_children_t`
Children from a node in a linked list.

*Public members*
- `cleri_node_t * cleri_children_t.node`: Child node. (readonly)
- `struct cleri_children_s * cleri_children_t.next`: Next child node or `NULL` if there are no other childs. (readonly)

Example looping over all children within a node:
```c
/* we asume having a node (cleri_node_t*) */
if (cleri_node_has_children(node)) {
    cleri_children_t * child = node->children;
    while (child != NULL) {
        // do something with child->node
        child = child->next;
    }
}
```

### `cleri_olist_t`
Linked list holding libcleri objects. A `cleri_olist_t` type is used for
expected elements in a parse result.

*Public members*
- `cleri_t * cl_obj`: Object (holding an element, readonly)
- `cleri_olist_t * next`: Next object. (readonly)

Example looping over `cleri_parse_t.expect`:
```c
/* we assume having a pr (cleri_parse_t*)
 *
 * Notes:
 *    pr->expect is NULL if nothing is expected and it is save to
 *    change pr->expect. If required the linked list can be reset to start
 *    using cleri_parse_expect_start(). */
while (pr->expect != NULL) {
    // do something with pr->expect->cl_obj
    pr->expect = pr->expect->next;
}
```
## Elements
Elements are objects used to define a grammar.

### `cleri_keyword_t`
Keyword element. The parser needs a match with the keyword.

*Type (`cleri_t.tp`)*: `CLERI_TP_KEYWORD`

*Public members*
- `const char * cleri_keyword_t.keyword`: Contains the keyword string. (readonly)
- `int cleri_keyword_t.ign_case`: Boolean. (readonly)
- `size_t cleri_keyword_t.len`: Length of the keyword string. (readonly)

#### `cleri_t * cleri_keyword(uint32_t gid, const char * keyword, int ign_case)`
Create and return a new [object](#cleri_t) containing a keyword element.
Argument `ign_case` can be set to 1 for a case insensitive keyword match.

Example:
```c
/* define case insensitive keyword */
cleri_t * k_tictactoe = cleri_keyword(
    0,                  // gid, not used in this example
    "tic-tac-toe",      // keyword
    1);                 // case insensitive

/* create grammar with custom keyword regular expression match */
cleri_grammar_t * grammar = cleri_grammar(k_tictactoe, "^[A-Za-z-]+");

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "Tic-Tac-Toe");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_regex_t`
Regular expression element. The parser needs a match with the regular
expression.

*No public members*

#### `cleri_t * cleri_regex(uint32_t gid, const char * pattern)`
Create and return a new [object](#cleri_t) containing a regular
expression element. Argument `pattern` should contain the regular expression.
Each pattern must start with character `^` and the pattern should be checked
before calling this function.

See [Quick usage](#quick-usage) for a `cleri_regex_t` example.

### `cleri_choice_t`
Choice element. The parser must choose one of the child elements.

*Public members*
- `int cleri_choice_t.most_greedy`: Boolean. (readonly)
- `cleri_olist_t * cleri_choice_t.olist`: Children. (readonly)

#### `cleri_t * cleri_choice(uint32_t gid, int most_greedy, size_t len, ...)`
Create and return a new [object](#cleri_t) containing a choice element.
Argument `most_greedy` can be set to 1 in which case the parser will select the
most greedy match. When 0, the parser will select the first match.

Example:
```c
/* define grammar */
cleri_t * k_hello = cleri_keyword(0, "hello", 0);
cleri_t * k_goodbye = cleri_keyword(0, "goodbye", 0);
cleri_t * choice = cleri_choice(
    0,                      // gid, not used in this example
    0,                      // stop at first match
    2,                      // number of elements
    k_hello, k_goodbye);    // elements

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(choice, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "goodbye");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_sequence_t`
Sequence element. The parser must match each element in the specified order.

*Public members*
- `cleri_olist_t * cleri_sequence_t.olist`: Elements. (readonly)

#### `cleri_t * cleri_sequence(uint32_t gid, size_t len, ...)`
Create and return a new [object](#cleri_t) containing a sequence element.

Example:
```c
cleri_t * sequence = cleri_sequence(
    0,                              // gid, not used in the example
    3,                              // number of elements
    cleri_keyword(0, "Tic", 0),     // first element
    cleri_keyword(0, "Tac", 0),     // second element
    cleri_keyword(0, "Toe", 0));    // third element

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(sequence, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "Tic Tac Toe");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_optional_t`
Optional element. The parser looks for an optional element.

*Public members*
- `cleri_t * cleri_optional_t.cl_obj`: Optional element. (readonly)

#### `cleri_t * cleri_optional(uint32_t gid, cleri_t * cl_obj)`
Create and return a new [object](#cleri_t) containing an optional element.

Example:
```c
/* define grammar */
cleri_t * k_hello = cleri_keyword(0, "hello", 0);
cleri_t * k_there = cleri_keyword(0, "there", 0);
cleri_t * optional = cleri_optional(
    0,                  // gid, not used in this example
    k_there);           // optional element
cleri_t * greet = cleri_sequence(
    0,                  // gid, not used in this example
    2,                  // number of elements
    k_hello, optional); // elements

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(greet, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "hello");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_prio_t`
Prio element. The parser must match one element. Inside the prio element it
is possible to use `CLERI_THIS` which is a reference to itself.

>Note: Use a [forward reference](#forward-reference) when possible.
>A prio is required when the same position in a string is potentially checked
>more than once.

*Public members*
- `cleri_olist_t * cleri_sequence_t.olist`: Elements. (readonly)

#### `cleri_t * cleri_prio(uint32_t gid, size_t len, ...)`
Create and return a new [object](#cleri_t) containing a prio element.

Example:
```c
/*
 * define grammar.
 *
 * Note: The third and fourth element are using a reference to the prio
 *       element at the same position in the string as the prio element.
 *       This is why a forward reference cannot be used for this example.
 */
cleri_t * prio = cleri_prio(
    0,                              // gid, not used in the example
    4,                              // number of elements
    cleri_keyword(0, "ni", 0),      // first element
    cleri_sequence(0, 3,            // second element
        cleri_token(0, "("),
        CLERI_THIS,
        cleri_token(0, ")")),
    cleri_sequence(0, 3,            // third element
        CLERI_THIS,
        cleri_keyword(0, "or", 0),
        CLERI_THIS),
    cleri_sequence(0, 3,            // fourth element
        CLERI_THIS,
        cleri_keyword(0, "and", 0),
        CLERI_THIS));

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(prio, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "(ni or ni) and (ni or ni)");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_repeat_t`
Repeat element. The parser must math at least `cleri_repeat_t.min` elements and
at most `cleri_repeat_t.max`. An unlimited amount is allowed in case `cleri_repeat_t.max`
is set to 0 (zero).

*Public members*
- `cleri_t * cleri_repeat_t.cl_obj`: Element to repeat. (readonly)
- `size_t cleri_repeat_t.min`: Minimum times an element is expected. (readonly)
- `size_t cleri_repeat_t.max`: Maximum times an element is expected or 0 for unlimited. (readonly)

#### `cleri_t * cleri_repeat(uint32_t gid, cleri_t * cl_obj, size_t min, size_t max)`
Create and return a new [object](#cleri_t) containing a repeat element.
Argument `max` should be greater or equal to `min` or 0.

Example:
```c
/* define grammar */
cleri_t * repeat = cleri_repeat(
    0,                          // gid, not used in this example
    cleri_keyword(0, "ni", 0),  // repeated element
    0,                          // min n times
    0);                         // max n times (0 for unlimited)

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(repeat, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "ni ni ni ni ni");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_list_t`
List element. Like [repeat](#cleri_repeat_t) but with a delimiter.

*Public members*
- `cleri_t * cleri_list_t.cl_obj`: Element to repeat. (readonly)
- `cleri_t * cleri_list_t.delimiter`: Delimiter between repeating element. (readonly)
- `size_t cleri_list_t.min`: Minimum times an element is expected. (readonly)
- `size_t cleri_list_t.max`: Maximum times an element is expected or 0 for unlimited. (readonly)
- `int cleri_list_t.opt_closing`: Allow or disallow ending with a delimiter.


#### `cleri_t * cleri_list(uint32_t gid, cleri_t * cl_obj, cleri_t * delimiter, size_t min, size_t max, int opt_closing)`
Create and return a new [object](#cleri_t) containing a list element.
Argument `max` should be greater or equal to `min` or 0. Argument `opt_closing`
can be 1 (TRUE) to allow or 0 (FALSE) to disallow a list to end with a delimiter.

Example:
```c
/* define grammar */
cleri_t * list = cleri_list(
    0,                          // gid, not used in this example
    cleri_keyword(0, "ni", 0),  // repeated element
    cleri_token(0, ","),        // delimiter element
    0,                          // min n times
    0,                          // max n times (0 for unlimited)
    0);                         // disallow ending with a delimiter

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(list, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "ni, ni, ni, ni, ni");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_token_t`
Token element. The parser must math a token exactly. A token can be one or more
characters and is usually used to match operators like `+`, `-`, `*` etc.

*Public members*
- `const char * cleri_token_t.token`: Token string. (readonly)
- `size_t cleri_token_t.len`: Length of the token string. (readonly)

#### `cleri_t * cleri_token(uint32_t gid, const char * token)`
Create and return a new [object](#cleri_t) containing a token element.

Example:
```c
/* define grammar */
cleri_t * token = cleri_token(
    0,          // gid, not used in this example
    "-");       // token string (dash)

cleri_t * ni =  cleri_keyword(0, "ni", 0);
cleri_t * list = cleri_list(0, ni, token, 0, 0, 0);

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(list, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "ni-ni - ni- ni -ni");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_tokens_t`
Tokens element. Can be used to register multiple tokens at once.

#### `cleri_t * cleri_tokens(uint32_t gid, const char * tokens)`
Create and return a new [object](#cleri_t) containing a tokens element.
Argument `tokens` must be a string with tokens seperated by spaces. If given
tokens are different in size, the parser will try to match the longest tokens
first.

Example:
```c
/* define grammar */
cleri_t * tokens = cleri_tokens(
    0,              // gid, not used in this example
    "+ - -=");      // tokens string '+', '-' and '-='

cleri_t * ni =  cleri_keyword(0, "ni", 0);
cleri_t * list = cleri_list(0, ni, tokens, 0, 0, 0);

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(list, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "ni + ni -= ni - ni");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `Forward reference`
Forward reference to a libcleri object. There is no specific type for a
reference.

>Warning: A reference is not protected against testing the same position in
>in a string. This could potentially lead to an infinite loop.
>For example:
>```c
>cleri_ref_set(ref, cleri_optional(0, ref)); // DON'T DO THIS
>```
>Use [prio](#cleri_prio_t) if such recursive construction is required.

#### `cleri_t * cleri_ref(void)`
Create and return a new [object](#cleri_t) as reference element.
Once the reference is created, it can be used as element in you grammar. Do not
forget to actually set the reference using `cleri_ref_set()`.

#### `void cleri_ref_set(cleri_t * ref, cleri_t * cl_obj)`
Set a reference. For every created forward reference, this function must be
called exactly once. Argument `ref` must be created with `cleri_ref()`. Argument
`cl_obj` cannot be used outside the reference. Since the reference becomes
the `cl_obj`, it is the reference you should use.

Example
```c
/* define grammar */
cleri_t * ref = cleri_ref();
cleri_t * choice = cleri_choice(
    0, 0, 2, cleri_keyword(0, "ni", 0), ref);

cleri_ref_set(ref, cleri_sequence(
    0,
    3,
    cleri_token(0, "["),
    cleri_list(0, choice, cleri_token(0, ","), 0, 0, 0),
    cleri_token(0, "]")));

/* create grammar */
cleri_grammar_t * grammar = cleri_grammar(ref, NULL);

/* parse some test string */
cleri_parse_t * pr = cleri_parse(grammar, "[ni, ni, [ni, [], [ni, ni]]]");
printf("Valid: %s\n", pr->is_valid ? "true" : "false"); // true

/* cleanup */
cleri_parse_free(pr);
cleri_grammar_free(grammar);
```

### `cleri_dup_t`
Duplicate an object. The type is an extension to `cleri_t`.

#### `cleri_t * cleri_dup(uint32_t gid, cleri_t * cl_obj)`
Duplicate a libcleri object with a different gid but using the same element.

>Note: Only the object is duplicated. The element (`cleri_via_t via`)
>is a pointer to the original object.

The following [pyleri](https://github.com/transceptor-technology/pyleri) code
will use `cleri_dup()` when exported to c:
```python
elem = Repeat(obj, mi=1, ma=1)
```

Use the code below if you want similar behavior without duplication:
```python
elem = Sequence(obj)
```

### Miscellaneous functions
#### `const char * cleri_version(void)`
Returns the version of libcleri.
