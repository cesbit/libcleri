#include <stdio.h>
#include <cleri/object.h>

const char * TestKeyword = "Tic-Tac-Toe";

int main(void)
{
    /* define keyword */
    cleri_object_t * k_tictactoe = cleri_keyword(0, "tic-tac-toe", 1);

    /* create grammar with custom keyword regular expression match */
    cleri_grammar_t * grammar = cleri_grammar(k_tictactoe, "^[A-Za-z-]+");

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestKeyword);
    printf("Test '%s': %s\n", TestKeyword, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}