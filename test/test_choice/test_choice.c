#include "../test.h"
#include "../helpers.h"


static int test_choice_most_greedy(void)
{
    test_start("choice (most_greedy)");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_iris, * seq, * choice;

    k_hi = cleri_keyword(0, "hi", 0);
    k_iris = cleri_keyword(0, "iris", 0);
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

    k_hi = cleri_keyword(0, "hi", 0);
    k_iris = cleri_keyword(0, "iris", 0);
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

int main()
{
    return (
        test_choice_most_greedy() ||
        test_choice_first_match() ||
        0
    );
}
