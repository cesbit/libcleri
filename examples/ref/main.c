#include <stdio.h>
#include <cleri/object.h>

const char * TestRef = "[ni, ni, [ni, [], [ni, ni]]]";

int main(void)
{
    /* define grammar */
    cleri_object_t * ref = cleri_ref();
    cleri_object_t * choice = cleri_choice(
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
    cleri_parse_t * pr = cleri_parse(grammar, TestRef);
    printf("Test '%s': %s\n", TestRef, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}