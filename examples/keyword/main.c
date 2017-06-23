#include <stdio.h>
#include <cleri/cleri.h>

const char * TestKeyword = "Tic-Tac-Toe";

int main(void)
{
    /* define keyword */
    cleri_t * k_tictactoe = cleri_keyword(
        0,                  // gid, not used in this example
        "tic-tac-toe",      // keyword
        1);                 // case insensitive

    /* create grammar with custom keyword regular expression match */
    cleri_grammar_t * grammar = cleri_grammar(k_tictactoe, "^[A-Za-z-]+");

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestKeyword);
    printf("Test: %s, '%s'\n", pr->is_valid ? "true" : "false", TestKeyword);

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}