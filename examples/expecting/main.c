#include <stdio.h>
#include <cleri/cleri.h>
#include "expecting.h"



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

const char * re_to_str(uint32_t gid)
{
    switch (gid)
    {
        case CLERI_GID_R_DOUBLEQ_STR:   return "double quote string";
        case CLERI_GID_R_FLOAT:         return "float";
        case CLERI_GID_R_GRAVE_STR:     return "grave string";
        case CLERI_GID_R_INTEGER:       return "integer";
        case CLERI_GID_R_REGEX:         return "regex";
        case CLERI_GID_R_SINGLEQ_STR:   return "single quote string";
        case CLERI_GID_R_TIME_STR:      return "date/time";
        case CLERI_GID_R_UINTEGER:      return "unidentified integer";
        default:                        return "Not a regex";
    }
}

void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s'\n", str);
    printf("The string is %s\n", pr->is_valid ? "valid" : "NOT valid");
    if (!pr->is_valid)
    {
        printf("The error occurred at position %lu.\n", pr->pos);
    }

    expecting_str(grammar, str);
    cleri_parse_free(pr);
}

int main(void)
{
        /* define grammar */
    cleri_t * r_float = cleri_regex(CLERI_GID_R_FLOAT,
                                    CLERI_REGEXSTR_FLOAT);
    cleri_t * r_integer = cleri_regex(CLERI_GID_R_INTEGER,
                                      CLERI_REGEXSTR_INTEGER);
    // cleri_t * r_uinteger = cleri_regex(CLERI_GID_R_UINTEGER,
    //                                    CLERI_REGEXSTR_UINTEGER);
    cleri_t * r_time_str = cleri_regex(CLERI_GID_R_TIME_STR,
                                       CLERI_REGEXSTR_TIME_STR);
    cleri_t * r_singleq_str = cleri_regex(CLERI_GID_R_SINGLEQ_STR,
                                          CLERI_REGEXSTR_SINGLEQ_STR);
    cleri_t * r_doubleq_str = cleri_regex(CLERI_GID_R_DOUBLEQ_STR,
                                          CLERI_REGEXSTR_DOUBLEQ_STR);
    // cleri_t * r_grave_str = cleri_regex(CLERI_GID_R_GRAVE_STR,
    //                                     CLERI_REGEXSTR_GRAVE_STR);
    // cleri_t * r_regex = cleri_regex(CLERI_GID_R_REGEX,
    //                                 CLERI_REGEXSTR_REGEX);

    cleri_t * k_hi = cleri_keyword(0, "hi", 0);
    cleri_t * k_bye = cleri_keyword(0, "bye", 0);
    cleri_t * start = cleri_repeat(0,
                                   cleri_sequence(0,
                                                  2,
                                                  cleri_choice(0,
                                                               0,
                                                               2,
                                                               k_hi,
                                                               k_bye),
                                                  cleri_choice(0,
                                                               0,
                                                               5,
                                                               r_float,
                                                               r_integer,
                                                               r_time_str,
                                                               r_singleq_str,
                                                               r_doubleq_str)),
                                   0,
                                   0);

    /* compile grammar */
    cleri_grammar_t * my_grammar = cleri_grammar(start, NULL);

    /* test some strings */
    char str[30];
    fgets(str, sizeof(char) * 30, stdin);
    test_str(my_grammar, (const char *)str);  // true

    /* cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}

