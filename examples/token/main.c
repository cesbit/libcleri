#include <stdio.h>
#include <cleri/cleri.h>

const char * TestToken = "ni-ni - ni- ni -ni";

int main(void)
{
    /* define grammar */
    cleri_t * token = cleri_token(
        0,          // gid, not used in this example
        "-");       // token string (dash)

    cleri_t * ni =  cleri_keyword(0, "ni", 0);
    cleri_t * list = cleri_list(0, ni, token, 0, 0, 0);

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(list, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestToken);
    printf("Test: %s, '%s'\n", pr->is_valid ? "true" : "false", TestToken);

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}
