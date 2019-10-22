#include "../test.h"
#include "../helpers.h"


static int test_token(void)
{
    test_start("token");

    cleri_grammar_t * grammar;
    cleri_t * dot;

    dot = cleri_token(0, ".");
    grammar = cleri_grammar(dot, NULL);

    // assert statements
    _assert_is_valid(grammar, ".");
    _assert_is_not_valid(grammar, "..");
    _assert_is_not_valid(grammar, "");
    _assert_parse_str (
        grammar,
        "",
        "error at line 0, position 0, expecting: .",
        NULL);
    _assert_parse_str2 (
        grammar,
        "",
        "error at line 0, position 0",
        NULL);

    cleri_grammar_free(grammar);

    return test_end();
}

static int test_token_multi_char(void)
{
    test_start("token (multi_char)");

    cleri_grammar_t * grammar;
    cleri_t * not;

    not = cleri_token(0, "!=");
    grammar = cleri_grammar(not, NULL);

    // assert statements
    _assert_is_valid(grammar, " != ");
    _assert_is_not_valid(grammar, "!");
    _assert_is_not_valid(grammar, "");
    _assert_parse_str (
        grammar,
        "",
        "error at line 0, position 0, expecting: !=",
        NULL);
    _assert_parse_str2 (
        grammar,
        "",
        "error at line 0, position 0",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_token() ||
        test_token_multi_char() ||
        0
    );
}
