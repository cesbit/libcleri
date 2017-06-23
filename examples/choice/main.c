#include <stdio.h>
#include <cleri/cleri.h>

const char * TestChoice = "goodbye";

int main(void)
{
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
    cleri_parse_t * pr = cleri_parse(grammar, TestChoice);
    printf("Test: %s, '%s'\n", pr->is_valid ? "true" : "false", TestChoice);

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}