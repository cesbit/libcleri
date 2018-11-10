#include "../test.h"
#include "../helpers.h"


static int test_sequence(void)
{
    test_start("sequence");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_iris, * seq;

    k_hi = cleri_keyword(0, "hi", false);
    k_iris = cleri_keyword(0, "iris", false);
    seq = cleri_sequence(0, 2, k_hi, k_iris);
    grammar = cleri_grammar(seq, NULL);

    _assert_is_valid (grammar, "hi iris");
    _assert_is_not_valid (grammar, "hi sasha");
    _assert_parse_str (
        grammar,
        "hi sasha",
        "error at position 3, expecting: iris",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hi sasha",
        "error at position 3",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_sequence() ||
        0
    );
}
