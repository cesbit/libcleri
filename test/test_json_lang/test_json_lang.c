#include "../test.h"
#include "../helpers.h"

enum cleri_grammar_ids {
    CLERI_NONE,   // used for objects with no name
    CLERI_GID_JSON_ARRAY,
    CLERI_GID_JSON_MAP,
    CLERI_GID_JSON_MAP_ITEM,
    CLERI_GID_K_FALSE,
    CLERI_GID_K_NULL,
    CLERI_GID_K_TRUE,
    CLERI_GID_R_FLOAT,
    CLERI_GID_R_INTEGER,
    CLERI_GID_R_STRING,
    CLERI_GID_START,
    CLERI_END // can be used to get the enum length
};

#define CLERI_CASE_SENSITIVE 0
#define CLERI_CASE_INSENSITIVE 1

#define CLERI_FIRST_MATCH 0
#define CLERI_MOST_GREEDY 1

cleri_grammar_t * compile_grammar(void)
{
    cleri_t * START = cleri_ref();
    cleri_t * r_string = cleri_regex(CLERI_GID_R_STRING, "^(\")(?:(?=(\\\\?))\\2.)*?\\1");
    cleri_t * r_float = cleri_regex(CLERI_GID_R_FLOAT, "^-?[0-9]+\\.?[0-9]+");
    cleri_t * r_integer = cleri_regex(CLERI_GID_R_INTEGER, "^-?[0-9]+");
    cleri_t * k_true = cleri_keyword(CLERI_GID_K_TRUE, "true", CLERI_CASE_SENSITIVE);
    cleri_t * k_false = cleri_keyword(CLERI_GID_K_FALSE, "false", CLERI_CASE_SENSITIVE);
    cleri_t * k_null = cleri_keyword(CLERI_GID_K_NULL, "null", CLERI_CASE_SENSITIVE);
    cleri_t * json_map_item = cleri_sequence(
        CLERI_GID_JSON_MAP_ITEM,
        3,
        r_string,
        cleri_token(CLERI_NONE, ":"),
        START
    );
    cleri_t * json_map = cleri_sequence(
        CLERI_GID_JSON_MAP,
        3,
        cleri_token(CLERI_NONE, "{"),
        cleri_list(CLERI_NONE, json_map_item, cleri_token(CLERI_NONE, ","), 0, 0, 0),
        cleri_token(CLERI_NONE, "}")
    );
    cleri_t * json_array = cleri_sequence(
        CLERI_GID_JSON_ARRAY,
        3,
        cleri_token(CLERI_NONE, "["),
        cleri_list(CLERI_NONE, START, cleri_token(CLERI_NONE, ","), 0, 0, 0),
        cleri_token(CLERI_NONE, "]")
    );
    cleri_ref_set(START, cleri_choice(
        CLERI_GID_START,
        CLERI_MOST_GREEDY,
        8,
        r_string,
        r_float,
        r_integer,
        k_true,
        k_false,
        k_null,
        json_map,
        json_array
    ));

    cleri_grammar_t * grammar = cleri_grammar(START, "^\\w+");

    return grammar;
}


static int test_json_lang(void)
{
    test_start("json (language)");

    cleri_grammar_t * grammar = compile_grammar();
    _assert (grammar);

    _assert_is_valid (grammar, "{\"json\": [1, 2, 3]}");
    _assert_is_valid (grammar, "{\"json\": {\"isOk\": true}}");
    _assert_is_not_valid (grammar, "{\"json\": [1, 2, 3]");

    cleri_grammar_free(grammar);

    return test_end();
}


int main()
{
    return (
        test_json_lang() ||
        0
    );
}