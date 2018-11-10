#include "../test.h"
#include "../helpers.h"

#define id 5

static const char * translate(cleri_t * o)
{
    switch (o->gid)
    {
    case id:
        return "single_quoted_string";
    }
    return NULL;
}


static int test_regex(void)
{
    test_start("regex");

    cleri_grammar_t * grammar;
    cleri_t * regex;

    regex = cleri_regex(id, "^(?:\'(?:[^\']*)\')+");
    _assert (regex);
    grammar = cleri_grammar(regex, NULL);

    _assert (grammar);
    _assert (regex->gid == id);
    _assert_is_valid (grammar, "'hi'");
    _assert_is_valid (grammar, "'this is ''SiriDB'''");
    _assert_is_not_valid (grammar, "'Hi !");
    _assert_is_not_valid (grammar, "'hello''");
    _assert_is_not_valid (grammar, "");
    _assert_parse_str (
        grammar,
        "\"double quoted\"",
        "error at position 0, expecting: single_quoted_string",
        translate);
    _assert_parse_str2 (
        grammar,
        "\"double quoted\"",
        "error at position 0",
        translate);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_regex() ||
        0
    );
}
