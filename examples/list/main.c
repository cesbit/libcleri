#include <stdio.h>
#include <cleri/object.h>

const char * TestList = "ni, ni, ni, ni, ni";

int main(void)
{
    /* define grammar */
    cleri_object_t * list = cleri_list(
        0,                          // gid, not used in this example
        cleri_keyword(0, "ni", 0),  // repeated element
        cleri_token(0, ","),        // delimiter element
        0,                          // min n times
        0,                          // max n times (0 for unlimited)
        0);                         // disallow ending with a delimiter

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(list, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestList);
    printf("Test '%s': %s\n", TestList, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}
