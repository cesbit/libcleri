#include "../test.h"
#include "../helpers.h"


static const char * translate(cleri_t * o __attribute__((unused)))
{
    /* this should ignore expecting: ... */
    return "";
}

static int test_repeat(void)
{
    test_start("repeat");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * repeat;

    k_hi = cleri_keyword(0, "hi", false);
    repeat = cleri_repeat(0, k_hi, 0, 0);
    grammar = cleri_grammar(repeat, NULL);

    // assert statements
    _assert (repeat->via.repeat->min == 0);
    _assert (repeat->via.repeat->max == 0);
    _assert_is_valid (grammar, "hi hi hi");
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "");
    _assert_is_not_valid (grammar, "hihi");
    _assert_is_not_valid (grammar, "ha");
    _assert_parse_str (
        grammar,
        "hi.",
        "error at position 2, expecting: hi or end_of_statement",
        NULL);
    _assert_parse_str (
        grammar,
        "hi.",
        "error at position 2",
        &translate);
    _assert_parse_str2 (
        grammar,
        "hi.",
        "error at position 2",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hi.",
        "error at position 2",
        &translate);
    cleri_grammar_free(grammar);

    return test_end();
}

static int test_repeat_all_options(void)
{
    test_start("repeat (all_options)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * repeat;

    k_hi = cleri_keyword(0, "hi", false);
    repeat = cleri_repeat(0, k_hi, 1, 3);
    grammar = cleri_grammar(repeat, NULL);

    // assert statements
    _assert (repeat->via.repeat->min == 1);
    _assert (repeat->via.repeat->max == 3);
    _assert_is_valid (grammar, "hi  hi hi");
    _assert_is_valid (grammar, "hi");
    _assert_is_not_valid (grammar, "");
    _assert_is_not_valid (grammar, "hi hi hi hi");
    _assert_parse_str (
        grammar,
        "hi hi hi hi hi",
        "error at position 8, expecting: end_of_statement",
        NULL);
    _assert_parse_str (
        grammar,
        "hi.",
        "error at position 2, expecting: hi or end_of_statement",
        NULL);
    _assert_parse_str (
        grammar,
        "",
        "error at position 0, expecting: hi",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hi hi hi hi hi",
        "error at position 8",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hi.",
        "error at position 2",
        NULL);
    _assert_parse_str2 (
        grammar,
        "",
        "error at position 0",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_repeat() ||
        test_repeat_all_options() ||
        0
    );
}
