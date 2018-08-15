#include <stdio.h>
#include <cleri/cleri.h>
#include <time.h>
//int view_str(cleri_children_t * child, cleri_tp tp);
//void test_str(cleri_grammar_t * grammar, const char * str);


enum cleri_grammar_ids {
    CLERI_GID_R_COMMENT,
    CLERI_GID_R_DOUBLEQ_STR,
    CLERI_GID_R_FLOAT,
    CLERI_GID_R_GRAVE_STR,
    CLERI_GID_R_INTEGER,
    CLERI_GID_R_REGEX,
    CLERI_GID_R_SINGLEQ_STR,
    CLERI_GID_R_TIME_STR,
    CLERI_GID_R_UINTEGER,
    CLERI_GID_R_UUID_STR,
};


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
                expect = "regex";
                switch (pr->expect->cl_obj->gid)
                {
                    case CLERI_GID_R_COMMENT:
                        expect = "comment";
                        break;
                    case CLERI_GID_R_DOUBLEQ_STR:
                        expect = "double quote string";
                        break;
                    case CLERI_GID_R_FLOAT:
                        expect = "float";
                        break;
                    case CLERI_GID_R_GRAVE_STR:
                        expect = "grave string";
                        break;
                    case CLERI_GID_R_INTEGER:
                        expect = "integer";
                        break;
                    case CLERI_GID_R_REGEX:
                        expect = "regex";
                        break;
                    case CLERI_GID_R_SINGLEQ_STR:
                        expect = "single quote string";
                        break;
                    case CLERI_GID_R_TIME_STR:
                        expect = "date/time";
                        break;
                    case CLERI_GID_R_UINTEGER:
                        expect = "unidentified integer";
                        break;
                    case CLERI_GID_R_UUID_STR:
                        expect = "uuid";
                        break;
                    default:
                        expect = "Not a regex";
                        break;
                }
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
    cleri_t * r_float = cleri_regex(CLERI_GID_R_FLOAT, "^[-+]?[0-9]*\\.?[0-9]+");
    cleri_t * r_integer = cleri_regex(CLERI_GID_R_INTEGER, "^[-+]?[0-9]+");
    // cleri_t * r_uinteger = cleri_regex(CLERI_GID_R_UINTEGER, "^[0-9]+");
    cleri_t * r_time_str = cleri_regex(CLERI_GID_R_TIME_STR, "^[0-9]+[smhdw]");
    cleri_t * r_singleq_str = cleri_regex(CLERI_GID_R_SINGLEQ_STR, "^(?:\'(?:[^\']*)\')+");
    cleri_t * r_doubleq_str = cleri_regex(CLERI_GID_R_DOUBLEQ_STR, "^(?:\"(?:[^\"]*)\")+");
    // cleri_t * r_grave_str = cleri_regex(CLERI_GID_R_GRAVE_STR, "^(?:`(?:[^`]*)`)+");
    // cleri_t * r_uuid_str = cleri_regex(CLERI_GID_R_UUID_STR, "^[0-9a-f]{8}\\-[0-9a-f]{4}\\-[0-9a-f]{4}\\-[0-9a-f]{4}\\-[0-9a-f]{12}");
    // cleri_t * r_regex = cleri_regex(CLERI_GID_R_REGEX, "^(/[^/\\\\]*(?:\\\\.[^/\\\\]*)*/i?)");
    // cleri_t * r_comment = cleri_regex(CLERI_GID_R_COMMENT, "^#.*");



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
    char str[50];
    scanf("%s", str);
    test_str(my_grammar, str);  // true

    /* cleanup grammar */
    cleri_grammar_free(my_grammar);

    return 0;
}

