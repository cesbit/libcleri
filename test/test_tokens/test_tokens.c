#include "../test.h"
#include "../helpers.h"


static int test_tokens(void)
{
    test_start("tokens");

    cleri_grammar_t * grammar;
    cleri_t * tokens;
    /* tokes will be ordered on length */
    const char * spaced = "== != >= <= > <";

    tokens = cleri_tokens(0, "== > !=  <  >=  <= ");
    grammar = cleri_grammar(tokens, NULL);

    // assert statements
    _assert (strcmp(tokens->via.tokens->spaced, spaced) == 0);
    _assert_is_valid (grammar, "==");
    _assert_is_valid (grammar, "<=");
    _assert_is_valid (grammar, ">");
    _assert_is_not_valid (grammar, "");
    _assert_is_not_valid (grammar, "=");
    _assert_parse_str (
        grammar,
        "",
        "error at position 0, expecting: == != >= <= > <",
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
        test_tokens() ||
        0
    );
}
