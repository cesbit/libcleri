#include <stdio.h>
#include <cleri/object.h>

const char * TestOptional = "hello";

int main(void)
{
    /* define grammar */
    cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
    cleri_object_t * k_there = cleri_keyword(0, "there", 0);
    cleri_object_t * optional = cleri_optional(
        0,                  // gid, not used in this example
        k_there);           // optional element
    cleri_object_t * greet = cleri_sequence(
        0,                  // gid, not used in this example
        2,                  // number of elements
        k_hello, optional); // elements

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(greet, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestOptional);
    printf("Test '%s': %s\n", TestOptional, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}

