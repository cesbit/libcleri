#include <stdio.h>
#include <cleri/cleri.h>
#include <string.h>
#include "expecting.h"

/* Predefined gids for regex elements. To improve clarity in the re_to_str()
and cleri_regex() */
enum cleri_grammar_ids {
    CLERI_GID_R_FLOAT,
    CLERI_GID_R_INTEGER,
    CLERI_GID_R_UINTEGER,
    CLERI_GID_R_TIME_STR,
    CLERI_GID_R_SINGLEQ_STR,
    CLERI_GID_R_DOUBLEQ_STR,
    CLERI_GID_R_GRAVE_STR,
    CLERI_GID_R_REGEX,
};

/* User-defined function used in the show_cleri_obj(). Here the regex elemnts
are given a user-defined name based on the gid.
With "choice" you can return either the explanation of the regex or an example
of the regex. */
const char * re_to_str(uint32_t gid, int choice)
{
    switch (gid)
    {
        case CLERI_GID_R_DOUBLEQ_STR:
            return (choice) ? "double quote string" : "\"siri\"";
        case CLERI_GID_R_FLOAT:
            return (choice) ? "float" : "3.2";
        case CLERI_GID_R_GRAVE_STR:
            return (choice) ? "grave string" : "`siri`";
        case CLERI_GID_R_INTEGER:
            return (choice) ? "integer" : "-3";
        case CLERI_GID_R_REGEX:
            return (choice) ? "regex" : "^(/[^/\\\\]*(?:\\\\.[^/\\\\]*)*/i?)";
        case CLERI_GID_R_SINGLEQ_STR:
            return (choice) ? "single quote string" : "'siri'";
        case CLERI_GID_R_TIME_STR:
            return (choice) ? "time" : "3s";
        case CLERI_GID_R_UINTEGER:
            return (choice) ? "unidentified integer" : "3";
        default:
            return (choice) ? "Not a regex" : "[whoops something went wrong]";
    }
}

cleri_grammar_t * create_grammar(void)
{
    /* Define grammar and if error occurred redirect to goto*/
    cleri_t * k_hi = cleri_keyword(0, "hi", 0);
    if (k_hi == NULL)
        goto end_create_grammar;
    cleri_t * k_bye = cleri_keyword(0, "bye", 0);
    if (k_bye == NULL)
        goto end_create_grammar;
    cleri_t * r_name = cleri_regex(CLERI_GID_R_DOUBLEQ_STR, CLERI_REGEXSTR_DOUBLEQ_STR);
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

/* On error: clean up the previous objects that were succesfully allocated */
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
    /* Creates grammar. If error occurred, NULL is returned and the program
    will be aborted. */
    cleri_grammar_t * my_grammar = create_grammar();
    if (my_grammar == NULL)
        abort();

    /* String given by user. */
    char str[30];
    fgets(str, sizeof(char) * 30, stdin);

    /* Replaces newline character with a 0 terminator. */
    int len=strlen(str);
    if(str[len-1]=='\n')
        str[len-1]='\0';

    /* Creates a dynamic buffer that will store the string to be parsed.
    If error occurred NULL is returned and redirected to goto. */
    buffer_t * buf = buffer_create();
        if (buf == NULL)
            goto end_main;

    /* String will be parsed and if invalid autocorrected */
    test_autocor(my_grammar, (const char *)str, buf);

    /* Cleanup*/
    buffer_destroy(buf);
end_main:
    cleri_grammar_free(my_grammar);

    return 0;
}
