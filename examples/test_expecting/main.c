#include <stdio.h>
#include <cleri/cleri.h>
#include <time.h>
//int view_str(cleri_children_t * child, cleri_tp tp);
//void test_str(cleri_grammar_t * grammar, const char * str);


// enum cleri_grammar_ids {
//     CLERI_GID_R_COMMENT,
//     CLERI_GID_R_DOUBLEQ_STR,
//     CLERI_GID_R_FLOAT,
//     CLERI_GID_R_GRAVE_STR,
//     CLERI_GID_R_INTEGER,
//     CLERI_GID_R_REGEX,
//     CLERI_GID_R_SINGLEQ_STR,
//     CLERI_GID_R_TIME_STR,
//     CLERI_GID_R_UINTEGER,
//     CLERI_GID_R_UUID_STR,
// };

// const char * re_to_str(uint32_t gid)
// {
//     const char * expect;
//     switch (gid)
//     {
//         case CLERI_GID_R_COMMENT:       return expect = "comment";
//         case CLERI_GID_R_DOUBLEQ_STR:   return expect = "double quote string";
//         case CLERI_GID_R_FLOAT:         return expect = "float";
//         case CLERI_GID_R_GRAVE_STR:     return expect = "grave string";
//         case CLERI_GID_R_INTEGER:       return expect = "integer";
//         case CLERI_GID_R_REGEX:         return expect = "regex";
//         case CLERI_GID_R_SINGLEQ_STR:   return expect = "single quote string";
//         case CLERI_GID_R_TIME_STR:      return expect = "date/time";
//         case CLERI_GID_R_UINTEGER:      return expect = "unidentified integer";
//         case CLERI_GID_R_UUID_STR:      return expect = "uuid";
//         default:                        return expect = "Not a regex";

//     }
// }

const char * re_to_str(uint32_t gid)
{
    const char * expect;
    switch (gid)
    {
        case 0:   return expect = "comment";
        case 1:   return expect = "double quote string";
        case 2:   return expect = "float";
        case 3:   return expect = "grave string";
        case 4:   return expect = "integer";
        case 5:   return expect = "regex";
        case 6:   return expect = "single quote string";
        case 7:   return expect = "date/time";
        case 8:   return expect = "unidentified integer";
        case 9:   return expect = "uuid";
        default:  return expect = "Not a regex";
    }
}


void test_str(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr = cleri_parse(grammar, str);
    printf("Test string '%s': %s\n", str, pr->is_valid ? "true" : "false");

    int count;
    const char * expect;

    for (count = 0; pr->expect != NULL; count++)
    {
        pr->expect = pr->expect->next;
    }

    if (count > 0)
    {
        int choice = (rand() * time(NULL)) % count; // seed
        printf("Choice: %d\n", choice);
        cleri_parse_expect_start(pr);
        while (choice--)
        {
            pr->expect = pr->expect->next; // takes choice.
        }
        switch(pr->expect->cl_obj->tp)
        {
            case CLERI_TP_END_OF_STATEMENT:
                expect = "end_of_statement";
                break;

            case CLERI_TP_KEYWORD:
                expect = pr->expect->cl_obj->via.keyword->keyword;
                break;

            case CLERI_TP_TOKENS:
                expect = pr->expect->cl_obj->via.tokens->tokens;
                break;

            case CLERI_TP_TOKEN:
                expect = pr->expect->cl_obj->via.token->token;
                break;

            case CLERI_TP_REGEX:
                expect = re_to_str(pr->expect->cl_obj->gid);
                break;

            default:
                expect = "not found";
        }

        printf("Expect: %s\n", expect);
    }

    cleri_parse_free(pr);
}


int main(void)
{
    /* define grammar */
    // cleri_t * r_float = cleri_regex(CLERI_GID_R_FLOAT, "^[-+]?[0-9]*\\.?[0-9]+");
    // cleri_t * r_integer = cleri_regex(CLERI_GID_R_INTEGER, "^[-+]?[0-9]+");
    // // cleri_t * r_uinteger = cleri_regex(CLERI_GID_R_UINTEGER, "^[0-9]+");
    // cleri_t * r_time_str = cleri_regex(CLERI_GID_R_TIME_STR, "^[0-9]+[smhdw]");
    // cleri_t * r_singleq_str = cleri_regex(CLERI_GID_R_SINGLEQ_STR, "^(?:\'(?:[^\']*)\')+");
    // cleri_t * r_doubleq_str = cleri_regex(CLERI_GID_R_DOUBLEQ_STR, "^(?:\"(?:[^\"]*)\")+");
    // // cleri_t * r_grave_str = cleri_regex(CLERI_GID_R_GRAVE_STR, "^(?:`(?:[^`]*)`)+");
    // // cleri_t * r_uuid_str = cleri_regex(CLERI_GID_R_UUID_STR, "^[0-9a-f]{8}\\-[0-9a-f]{4}\\-[0-9a-f]{4}\\-[0-9a-f]{4}\\-[0-9a-f]{12}");
    // // cleri_t * r_regex = cleri_regex(CLERI_GID_R_REGEX, "^(/[^/\\\\]*(?:\\\\.[^/\\\\]*)*/i?)");
    // // cleri_t * r_comment = cleri_regex(CLERI_GID_R_COMMENT, "^#.*");


    cleri_t * r_float = cleri_regex(2, "^[-+]?[0-9]*\\.?[0-9]+");
    cleri_t * r_integer = cleri_regex(4, "^[-+]?[0-9]+");
    cleri_t * r_time_str = cleri_regex(7, "^[0-9]+[smhdw]");
    cleri_t * r_singleq_str = cleri_regex(6, "^(?:\'(?:[^\']*)\')+");
    cleri_t * r_doubleq_str = cleri_regex(1, "^(?:\"(?:[^\"]*)\")+");

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
















