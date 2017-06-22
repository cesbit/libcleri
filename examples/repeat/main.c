#include <stdio.h>
#include <cleri/object.h>

const char * TestRepeat = "ni ni ni ni ni";

int main(void)
{
    /* define grammar */
    cleri_object_t * repeat = cleri_repeat(
        0,                          // gid, not used in this example
        cleri_keyword(0, "ni", 0),  // repeated element
        0,                          // min n times
        0);                         // max n times (0 for unlimited)

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(repeat, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestRepeat);
    printf("Test '%s': %s\n", TestRepeat, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}
