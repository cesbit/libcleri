#include <stdio.h>
#include <cleri/cleri.h>
#include "tree.h"



cleri_grammar_t * create_grammar(void)
{
    /* Define grammar and if error occurs, redirect (goto). */
    cleri_t * k_hi = cleri_keyword(0, "hi", 0);
    if (k_hi == NULL)
        goto end_create_grammar;
    cleri_t * k_bye = cleri_keyword(0, "bye", 0);
    if (k_bye == NULL)
        goto end_create_grammar;
    cleri_t * r_name = cleri_regex(0, "^(?:\"(?:[^\"]*)\")+");
    if (r_name == NULL)
        goto end_create_grammar;
    cleri_t * choice = cleri_choice(0, 0, 2, k_hi, k_bye);
    if (choice == NULL)
        goto end_create_grammar;
    cleri_t * seq = cleri_sequence(0, 2, choice, r_name);
    if (seq == NULL)
        goto end_create_grammar;
    cleri_t * start = cleri_repeat(0, seq, 0, 0);
    if (start == NULL)
        goto end_create_grammar;

    return cleri_grammar(start, NULL);

/* If error occurs, clean up the previous objects that were succesfully allocated. */
end_create_grammar:
    if (k_hi)
        cleri_free(k_hi);
    if (k_bye)
        cleri_free(k_bye);
    if (r_name)
        cleri_free(r_name);
    if (choice)
        cleri_free(choice);
    if (seq)
        cleri_free(seq);
    if (start)
        cleri_free(start);

    return NULL;
}

int main(void)
{
    /* Create grammar. If an error occurs, NULL is returned and
    the program will be aborted. */
    cleri_grammar_t * my_grammar = create_grammar();
    if (my_grammar == NULL)
        abort();

    /* Print the parse tree of the following string. */
    prt_tree(my_grammar, "hi \"Iris\" bye \"libcleri\"");  // true

    /* Cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}





