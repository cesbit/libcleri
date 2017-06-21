#include <stdio.h>
#include <cleri/object.h>

const char * TestChoice = "goodbye";

int main(void)
{
    /* define grammar */
    cleri_object_t * k_hello = cleri_keyword(0, "hello", 0);
    cleri_object_t * k_goodbye = cleri_keyword(0, "goodbye", 0);
    cleri_object_t * choice = cleri_choice(0, 0, 2, k_hello, k_goodbye);

    /* create grammar */
    cleri_grammar_t * grammar = cleri_grammar(choice, NULL);

    /* parse some test string */
    cleri_parse_t * pr = cleri_parse(grammar, TestChoice);
    printf("Test '%s': %s\n", TestChoice, pr->is_valid ? "true" : "false");

    /* cleanup */
    cleri_parse_free(pr);
    cleri_grammar_free(grammar);

    return 0;
}