#include <stdio.h>
#include <cleri/object.h>

const char * TestTokens = "ni + ni -= ni - ni";

int main(void)
{
    /* define grammar */
    cleri_object_t * tokens = cleri_tokens(
        0,              // gid, not used in this example
        "+ - -=");      // tokens string '+', '-' and '-='

    cleri_object_t * ni =  cleri_keyword(0, "ni", 0);
    cleri_object_t * list = cleri_list(0, ni, tokens, 0, 0, 0);

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(list, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestTokens);
    printf("Test '%s': %s\n", TestTokens, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}
