#include "../test.h"
#include "../helpers.h"


static int test_optional(void)
{
    test_start("optional");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * optional;

    k_hi = cleri_keyword(0, "hi", false);
    optional = cleri_optional(0, k_hi);
    grammar = cleri_grammar(optional, NULL);

    // assert statements
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "");
    _assert_is_not_valid(grammar, "hello");

    _assert_parse_str (
        grammar,
        "hello",
        "error at position 0, expecting: hi or end_of_statement",
        NULL);

    _assert_parse_str (
        grammar,
        "hi hi",
        "error at position 2, expecting: end_of_statement",
        NULL);

    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_optional() ||
        0
    );
}
