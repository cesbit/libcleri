#include "../test.h"
#include "../helpers.h"


static int test_keyword(void)
{
    test_start("keyword");

    cleri_grammar_t * grammar;
    cleri_t * k_hi;

    k_hi = cleri_keyword(0, "hi", false);
    _assert (k_hi);
    grammar = cleri_grammar(k_hi, NULL);
    _assert (grammar);

    _assert (k_hi->gid == 0);
    _assert (k_hi->via.keyword->ign_case == false);
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, " hi ");
    _assert_is_not_valid (grammar, "Hi");
    _assert_is_not_valid (grammar, "hello");
    _assert_parse_str (
        grammar,
        "hello",
        "error at line 1, position 0, unexpected `hello`, expecting: hi",
        NULL);
    _assert_parse_str (
        grammar,
        "hi",
        "parsed successfully",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hello",
        "error at line 1, position 0, unexpected `hello`",
        NULL);
    _assert_parse_str2 (
        grammar,
        "hi",
        "parsed successfully",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

static int test_keyword_ign_case(void)
{
    test_start("keyword (ign_case)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi;

    k_hi = cleri_keyword(1, "hi", true);
    _assert (k_hi);
    grammar = cleri_grammar(k_hi, NULL);
    _assert (grammar);

    _assert (k_hi->gid == 1);
    _assert (k_hi->via.keyword->ign_case == true);
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "Hi");
    _assert_is_not_valid (grammar, "hello");
    _assert_is_not_valid (grammar,
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi"
        "hihihihihihihihihihihihihihihihi");
    _assert_parse_str (
        grammar,
        "Hi Iris",
        "error at line 1, position 2, expecting: end_of_statement",
        NULL);
    _assert_parse_str2 (
        grammar,
        "Hi Iris",
        "error at line 1, position 2",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

static int test_keyword_alt_regkw(void)
{
    test_start("keyword (alt_regkw)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * t_hi;

    k_hi = cleri_keyword(0, "hi", false);
    t_hi = cleri_token(0, "HI");

    grammar = cleri_grammar(cleri_choice(0, 1, 2, k_hi, t_hi), "^[a-z]+");
    _assert (grammar);

    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "HI");
    _assert_is_not_valid (grammar, "Hi");

    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_keyword() ||
        test_keyword_ign_case() ||
        test_keyword_alt_regkw() ||
        0
    );
}
