#include <stdio.h>
#include <cleri/object.h>

const char * TestSequence = "Tic Tac Toe";

int main(void)
{
    /* define grammar */
    cleri_object_t * sequence = cleri_sequence(
        0,                              // gid, not used in the example
        3,                              // number of elements
        cleri_keyword(0, "Tic", 0),     // first element
        cleri_keyword(0, "Tac", 0),     // second element
        cleri_keyword(0, "Toe", 0));    // third element

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(sequence, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestSequence);
    printf("Test '%s': %s\n", TestSequence, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}

