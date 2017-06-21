#include <stdio.h>
#include <cleri/object.h>

const char * TestPrio = "(ni or ni) and (ni or ni)";

int main(void)
{
    /*
    * define grammar.
    *
    * Note: The third and fourth element are using a reference to the prio
    *       element at the same position in the string as the prio element.
    *       This is why a forward reference cannot be used for this example.
    */
    cleri_object_t * prio = cleri_prio(
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
    cleri_parse_t * pr = cleri_parse(grammar, TestPrio);
    printf("Test '%s': %s\n", TestPrio, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}

