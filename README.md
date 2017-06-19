# C Left-Right Parser (libcleri)
Languange parser for the C program language.

---------------------------------------
  * [Installation](#installation)
  * [Related projects](#related-projects)
  * [API](#api)
    * [cleri_object_t](#cleri_object_t)
    * [Miscellaneous functions](#miscellaneous-functions)

---------------------------------------

## Installation
>Note: libcleri requires [pcre](http://www.pcre.org/)

Install debug or release version, in this example we will install the release version.
```
$ cd Release
```

Compile libcleri
```
$ make all
```

Install libcleri
```
$ sudo make install
```

> Note: run `sudo make uninstall` for removal.

## Related projects
- [pyleri](https://github.com/transceptor-technology/pyleri): Python parser (can export grammar to pyleri, cleri and jsleri)
- [jsleri](https://github.com/transceptor-technology/jsleri): JavaScript parser
- [goleri](https://github.com/transceptor-technology/goleri): Go parser

## Usage
>The recommended way to create a static grammar is to use [pyleri](https://github.com/transceptor-technology/pyleri) for
>writing the language and from there export the language to libcleri or other languages.

This is a simple example using libcleri:
```c
#include <stdio.h>
#include <cleri/object.h>

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");
    cleri_parse_free(pr);
}

int main(void)
{
    /* define grammar */
    cleri_object_t * k_hi = cleri_keyword(0, "hi", 0);
    cleri_object_t * r_name = cleri_regex(0, "^(?:\"(?:[^\"]*)\")+");
    cleri_object_t * start = cleri_sequence(0, 2, k_hi, r_name);

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

### `cleri_object_t`
Cleri object is the base object for each element.

*Public members*
- `uint32_t gid`: Global Identifier for the element. This GID is not required and
as a rule it should be set to 0 if not used. You can use the GID for identifiying
an element in a parse result. When exporting a Pyleri grammar, each named element
will have set the GID be default.
- `cleri_object_tp tp`: Type for the object.
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
- `cleri_object_via_t via`: Object.
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

#### `cleri_object_t * cleri_object_new(uint32_t gid, cleri_object_tp tp, cleri_free_object_t free_object, cleri_parse_object_t parse_object)`
Create and return a new cleri object. This function should only be used in case
you want to create your own custom element.

#### `void cleri_object_incref(cleri_object_t * cl_object)`
Increment the reference counter for an object. Should only be used in case you
want to write your own custom element.

#### `void cleri_object_decref(cleri_object_t * cl_object)`
Decrement referenct counter for a cleri object. When the no more references are
left the object will be destroyed. Use this function to cleanup after errors
have occurred. Do not use this function after the element has successfully been
added to another element or grammar.

Example strict error handling:
```c
cleri_grammar_t * compile_grammar(void)
{
    cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
    if (k_hello == NULL) {
        return NULL;
    }
    cleri_object_t * k_world = cleri_keyword(0, "world", 0);
    if (k_world == NULL) {
        cleri_object_decref(k_hello); // must cleanup k_hello
        return NULL;
    }
    cleri_object_t * hello_world = cleri_sequence(0, 2, k_hello, k_world);
    if (start == NULL) {
        cleri_object_decref(k_hello);
        cleri_object_decref(k_world);
        return NULL;
    }
    cleri_object_t * opt = cleri_optional(0, hello_world);
    if (opt == NULL) {
        /* we now must only cleanup hello_world since this sequence will
        * cleanup both keywords too. */
        cleri_object_decref(hello_world);
        return NULL;
    }
    cleri_grammar_t * grammar = cleri_grammar(opt, NULL);
    if (grammar == NULL) {
        cleri_object_decref(opt);
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
>chain the final grammar returns NULL in case somewhere an error has occurred.
>In this case you should usually abort the program.

