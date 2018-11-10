#include "../test.h"
#include "../helpers.h"


static int test_ref(void)
{
    test_start("ref");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * ref;

    k_hi = cleri_keyword(0, "hi", false);
    ref = cleri_ref();
    grammar = cleri_grammar(ref, NULL);

    /* now set the reference */
    cleri_ref_set(ref, k_hi);

    _assert_is_valid (grammar, "hi");
    _assert_is_not_valid (grammar, "");
    _assert_parse_str (
        grammar,
        "ha",
        "error at position 0, expecting: hi",
        NULL);
    _assert_parse_str2 (
        grammar,
        "ha",
        "error at position 0",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_ref() ||
        0
    );
}
