#include "../test.h"
#include "../helpers.h"

#define ORIG 0
#define DUPL 1

static const char * translate(cleri_t * o)
{
    switch (o->gid)
    {
    case DUPL:
        return "hi(dup)";
    }
    return NULL;
};

static int test_dup(void)
{
    test_start("dup");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * dup;

    k_hi = cleri_keyword(ORIG, "hi", false);
    dup = cleri_dup(DUPL, k_hi);
    grammar = cleri_grammar(cleri_sequence(0, 2, k_hi, dup), NULL);

    _assert (k_hi->gid == ORIG);
    _assert (dup->gid == DUPL);
    _assert_is_valid (grammar, "hi hi");
    _assert_is_not_valid (grammar, "hi");
    _assert_parse_str (
        grammar,
        "",
        "error at line 1, position 0, expecting: hi",
        &translate);
    _assert_parse_str (
        grammar,
        "hi",
        "error at line 1, position 2, expecting: hi(dup)",
        &translate);
    _assert_parse_str2 (
        grammar,
        "",
        "error at line 1, position 0",
        &translate);
    _assert_parse_str2 (
        grammar,
        "hi",
        "error at line 1, position 2",
        &translate);
    cleri_grammar_free(grammar);

    return test_end();
}


int main()
{
    return (
        test_dup() ||
        0
    );
}
