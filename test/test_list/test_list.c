#include "../test.h"
#include "../helpers.h"


static int test_list(void)
{
    test_start("list");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * delimiter, * list;

    k_hi = cleri_keyword(0, "hi", 0);
    delimiter = cleri_token(0, ",");
    list = cleri_list(0, k_hi, delimiter, 0, 0, false);
    grammar = cleri_grammar(list, NULL);

    // assert statements
    _assert (list->via.list->min == 0);
    _assert (list->via.list->max == 0);
    _assert (list->via.list->opt_closing == false);
    _assert_is_valid (grammar, "hi, hi, hi");
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "");
    _assert_is_not_valid (grammar, "hi,");
    _assert_parse_str (
        grammar,
        "hi.",
        "error at position 2, expecting , or end_of_statement",
        NULL);

    cleri_grammar_free(grammar);

    return test_end();
}

static int test_list_all_options(void)
{
    test_start("list (all_options)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * delimiter, * list;

    k_hi = cleri_keyword(0, "hi", 0);
    delimiter = cleri_token(0, "-");
    list = cleri_list(0, k_hi, delimiter, 1, 3, true);
    grammar = cleri_grammar(list, NULL);

    // assert statements
    _assert (list->via.list->min == 1);
    _assert (list->via.list->max == 3);
    _assert (list->via.list->opt_closing == true);
    _assert_is_valid (grammar, "hi - hi - hi");
    _assert_is_valid (grammar, "hi-hi-hi-");
    _assert_is_valid (grammar, "hi-");
    _assert_is_valid (grammar, "hi");
    _assert_is_not_valid (grammar, "");
    _assert_is_not_valid (grammar, "-");
    _assert_is_not_valid (grammar, "hi-hi-hi-hi");
    _assert_parse_str (
        grammar,
        "hi-hi-hi-hi-hi",
        "error at position 9, expecting end_of_statement",
        NULL);
    _assert_parse_str (
        grammar,
        "hi.",
        "error at position 2, expecting - or end_of_statement",
        NULL);
    _assert_parse_str (
        grammar,
        "",
        "error at position 0, expecting hi",
        NULL);

    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_list() ||
        test_list_all_options() ||
        0
    );
}
