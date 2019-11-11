#include "../test.h"
#include "../helpers.h"


static int test_choice_most_greedy(void)
{
    test_start("choice (most_greedy)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_iris, * seq, * choice;

    k_hi = cleri_keyword(0, "hi", false);
    k_iris = cleri_keyword(0, "iris", false);
    seq = cleri_sequence(0, 2, k_hi, k_iris);
    choice = cleri_choice(0, true, 2, k_hi, seq);
    grammar = cleri_grammar(choice, NULL);

    _assert (choice->via.choice->most_greedy == true);
    _assert_is_valid (grammar, "hi");
    _assert_is_valid (grammar, "hi iris");
    _assert_is_not_valid (grammar, "hi sasha");

    cleri_grammar_free(grammar);

    return test_end();
}

static int test_choice_first_match(void)
{
    test_start("choice (first_match)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_iris, * seq, * choice;

    k_hi = cleri_keyword(0, "hi", false);
    k_iris = cleri_keyword(0, "iris", false);
    seq = cleri_sequence(0, 2, k_hi, k_iris);
    choice = cleri_choice(0, false, 2, k_hi, seq);
    grammar = cleri_grammar(choice, NULL);

    _assert (choice->via.choice->most_greedy == false);
    _assert_is_valid (grammar, "hi");
    _assert_is_not_valid (grammar, "hi iris");
    _assert_is_not_valid (grammar, "hi sasha");


    cleri_grammar_free(grammar);

    return test_end();
}

static int test_choice_expecting(void)
{
    test_start("choice (expecting)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_iris, * k_sasha, * seq, * choice;

    k_hi = cleri_keyword(0, "hi", false);
    k_iris = cleri_keyword(0, "iris", false);
    k_sasha = cleri_keyword(0, "sasha", false);
    choice = cleri_choice(0, false, 2, k_iris, k_sasha);
    seq = cleri_sequence(0, 2, k_hi, choice);
    grammar = cleri_grammar(seq, NULL);

    _assert (choice->via.choice->most_greedy == false);
    _assert_is_valid (grammar, "hi iris");
    _assert_is_valid (grammar, "hi sasha");
    _assert_is_not_valid (grammar, "hi cleri");

    _assert_parse_str (
        grammar,
        "hi cleri",
        "error at line 1, position 3, "
        "unexpected `cleri`, expecting: iris or sasha",
        NULL);

    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_choice_most_greedy() ||
        test_choice_first_match() ||
        test_choice_expecting() ||
        0
    );
}
