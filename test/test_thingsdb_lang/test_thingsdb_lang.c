#include "../test.h"
#include "../helpers.h"

enum cleri_grammar_ids {
    CLERI_NONE,   // used for objects with no name
    CLERI_GID_ARRAY,
    CLERI_GID_ASSIGN,
    CLERI_GID_BLOCK,
    CLERI_GID_CHAIN,
    CLERI_GID_CLOSURE,
    CLERI_GID_COMMENTS,
    CLERI_GID_END_STATEMENT,
    CLERI_GID_ENUM_,
    CLERI_GID_EXPRESSION,
    CLERI_GID_FOR_STATEMENT,
    CLERI_GID_FUNCTION,
    CLERI_GID_IF_STATEMENT,
    CLERI_GID_INDEX,
    CLERI_GID_INSTANCE,
    CLERI_GID_K_BREAK,
    CLERI_GID_K_CONTINUE,
    CLERI_GID_K_ELSE,
    CLERI_GID_K_FOR,
    CLERI_GID_K_IF,
    CLERI_GID_K_IN,
    CLERI_GID_K_RETURN,
    CLERI_GID_NAME,
    CLERI_GID_NAME_OPT_MORE,
    CLERI_GID_OPERATIONS,
    CLERI_GID_OPR0_MUL_DIV_MOD,
    CLERI_GID_OPR1_ADD_SUB,
    CLERI_GID_OPR2_BITWISE_AND,
    CLERI_GID_OPR3_BITWISE_XOR,
    CLERI_GID_OPR4_BITWISE_OR,
    CLERI_GID_OPR5_COMPARE,
    CLERI_GID_OPR6_CMP_AND,
    CLERI_GID_OPR7_CMP_OR,
    CLERI_GID_OPR8_TERNARY,
    CLERI_GID_PARENTHESIS,
    CLERI_GID_RETURN_STATEMENT,
    CLERI_GID_SLICE,
    CLERI_GID_START,
    CLERI_GID_STATEMENT,
    CLERI_GID_STATEMENTS,
    CLERI_GID_TEMPLATE,
    CLERI_GID_THING,
    CLERI_GID_T_FALSE,
    CLERI_GID_T_FLOAT,
    CLERI_GID_T_INT,
    CLERI_GID_T_NIL,
    CLERI_GID_T_REGEX,
    CLERI_GID_T_STRING,
    CLERI_GID_T_TRUE,
    CLERI_GID_VAR,
    CLERI_GID_VAR_OPT_MORE,
    CLERI_GID_X_ARRAY,
    CLERI_GID_X_ASSIGN,
    CLERI_GID_X_BLOCK,
    CLERI_GID_X_CHAIN,
    CLERI_GID_X_CLOSURE,
    CLERI_GID_X_FUNCTION,
    CLERI_GID_X_INDEX,
    CLERI_GID_X_PARENTHESIS,
    CLERI_GID_X_PREOPR,
    CLERI_GID_X_TERNARY,
    CLERI_GID_X_THING,
    CLERI_END // can be used to get the enum length
};

#define CLERI_CASE_SENSITIVE 0
#define CLERI_CASE_INSENSITIVE 1

#define CLERI_FIRST_MATCH 0
#define CLERI_MOST_GREEDY 1

cleri_grammar_t * compile_langdef(void)
{
    cleri_t * x_array = cleri_token(CLERI_GID_X_ARRAY, "[");
    cleri_t * x_assign = cleri_tokens(CLERI_GID_X_ASSIGN, "+= -= *= /= %= &= ^= |= =");
    cleri_t * x_block = cleri_token(CLERI_GID_X_BLOCK, "{");
    cleri_t * x_chain = cleri_token(CLERI_GID_X_CHAIN, ".");
    cleri_t * x_closure = cleri_token(CLERI_GID_X_CLOSURE, "|");
    cleri_t * x_function = cleri_token(CLERI_GID_X_FUNCTION, "(");
    cleri_t * x_index = cleri_token(CLERI_GID_X_INDEX, "[");
    cleri_t * x_parenthesis = cleri_token(CLERI_GID_X_PARENTHESIS, "(");
    cleri_t * x_preopr = cleri_regex(CLERI_GID_X_PREOPR, "^(\\s*!|\\s*[\\-+](?=[^0-9]))*");
    cleri_t * x_ternary = cleri_token(CLERI_GID_X_TERNARY, "?");
    cleri_t * x_thing = cleri_token(CLERI_GID_X_THING, "{");
    cleri_t * template = cleri_sequence(
        CLERI_GID_TEMPLATE,
        3,
        cleri_token(CLERI_NONE, "`"),
        cleri_repeat(CLERI_NONE, cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            2,
            cleri_regex(CLERI_NONE, "^([^`{}]|``|{{|}})+"),
            cleri_sequence(
                CLERI_NONE,
                3,
                cleri_token(CLERI_NONE, "{"),
                CLERI_THIS,
                cleri_token(CLERI_NONE, "}")
            )
        ), 0, 0),
        cleri_token(CLERI_NONE, "`")
    );
    cleri_t * t_false = cleri_keyword(CLERI_GID_T_FALSE, "false", CLERI_CASE_SENSITIVE);
    cleri_t * t_float = cleri_regex(CLERI_GID_T_FLOAT, "^[-+]?(inf|nan|[0-9]*\\.[0-9]+(e[+-][0-9]+)?)(?![0-9A-Za-z_])");
    cleri_t * t_int = cleri_regex(CLERI_GID_T_INT, "^[-+]?((0b[01]+)|(0o[0-8]+)|(0x[0-9a-fA-F]+)|([0-9]+))(?![0-9A-Za-z_])");
    cleri_t * t_nil = cleri_keyword(CLERI_GID_T_NIL, "nil", CLERI_CASE_SENSITIVE);
    cleri_t * t_regex = cleri_regex(CLERI_GID_T_REGEX, "^/((?:.(?!(?<![\\\\])/))*.?)/[a-z]*");
    cleri_t * t_string = cleri_regex(CLERI_GID_T_STRING, "^(((?:\'(?:[^\']*)\')+)|((?:\"(?:[^\"]*)\")+))");
    cleri_t * t_true = cleri_keyword(CLERI_GID_T_TRUE, "true", CLERI_CASE_SENSITIVE);
    cleri_t * comments = cleri_repeat(CLERI_GID_COMMENTS, cleri_choice(
        CLERI_NONE,
        CLERI_FIRST_MATCH,
        2,
        cleri_regex(CLERI_NONE, "^(?s)//.*?(\\r?\\n|$)"),
        cleri_regex(CLERI_NONE, "^(?s)/\\*.*?\\*/")
    ), 0, 0);
    cleri_t * name = cleri_regex(CLERI_GID_NAME, "^[A-Za-z_][0-9A-Za-z_]{0,254}(?![0-9A-Za-z_])");
    cleri_t * var = cleri_regex(CLERI_GID_VAR, "^[A-Za-z_][0-9A-Za-z_]{0,254}(?![0-9A-Za-z_])");
    cleri_t * chain = cleri_ref();
    cleri_t * closure = cleri_sequence(
        CLERI_GID_CLOSURE,
        4,
        x_closure,
        cleri_list(CLERI_NONE, var, cleri_token(CLERI_NONE, ","), 0, 0, 1),
        cleri_token(CLERI_NONE, "|"),
        CLERI_THIS
    );
    cleri_t * thing = cleri_sequence(
        CLERI_GID_THING,
        3,
        x_thing,
        cleri_list(CLERI_NONE, cleri_sequence(
            CLERI_NONE,
            3,
            name,
            cleri_token(CLERI_NONE, ":"),
            CLERI_THIS
        ), cleri_token(CLERI_NONE, ","), 0, 0, 1),
        cleri_token(CLERI_NONE, "}")
    );
    cleri_t * array = cleri_sequence(
        CLERI_GID_ARRAY,
        3,
        x_array,
        cleri_list(CLERI_NONE, CLERI_THIS, cleri_token(CLERI_NONE, ","), 0, 0, 1),
        cleri_token(CLERI_NONE, "]")
    );
    cleri_t * function = cleri_sequence(
        CLERI_GID_FUNCTION,
        3,
        x_function,
        cleri_list(CLERI_NONE, CLERI_THIS, cleri_token(CLERI_NONE, ","), 0, 0, 1),
        cleri_token(CLERI_NONE, ")")
    );
    cleri_t * instance = cleri_dup(CLERI_GID_INSTANCE, thing);
    cleri_t * enum_ = cleri_sequence(
        CLERI_GID_ENUM_,
        3,
        x_thing,
        cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            2,
            name,
            closure
        ),
        cleri_token(CLERI_NONE, "}")
    );
    cleri_t * opr0_mul_div_mod = cleri_tokens(CLERI_GID_OPR0_MUL_DIV_MOD, "* / %");
    cleri_t * opr1_add_sub = cleri_tokens(CLERI_GID_OPR1_ADD_SUB, "+ -");
    cleri_t * opr2_bitwise_and = cleri_tokens(CLERI_GID_OPR2_BITWISE_AND, "&");
    cleri_t * opr3_bitwise_xor = cleri_tokens(CLERI_GID_OPR3_BITWISE_XOR, "^");
    cleri_t * opr4_bitwise_or = cleri_tokens(CLERI_GID_OPR4_BITWISE_OR, "|");
    cleri_t * opr5_compare = cleri_tokens(CLERI_GID_OPR5_COMPARE, "== != <= >= < >");
    cleri_t * opr6_cmp_and = cleri_token(CLERI_GID_OPR6_CMP_AND, "&&");
    cleri_t * opr7_cmp_or = cleri_token(CLERI_GID_OPR7_CMP_OR, "||");
    cleri_t * opr8_ternary = cleri_sequence(
        CLERI_GID_OPR8_TERNARY,
        3,
        x_ternary,
        CLERI_THIS,
        cleri_token(CLERI_NONE, ":")
    );
    cleri_t * operations = cleri_sequence(
        CLERI_GID_OPERATIONS,
        3,
        CLERI_THIS,
        cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            9,
            opr8_ternary,
            opr7_cmp_or,
            opr6_cmp_and,
            opr5_compare,
            opr4_bitwise_or,
            opr3_bitwise_xor,
            opr2_bitwise_and,
            opr1_add_sub,
            opr0_mul_div_mod
        ),
        CLERI_THIS
    );
    cleri_t * assign = cleri_sequence(
        CLERI_GID_ASSIGN,
        2,
        x_assign,
        CLERI_THIS
    );
    cleri_t * name_opt_more = cleri_sequence(
        CLERI_GID_NAME_OPT_MORE,
        2,
        name,
        cleri_optional(CLERI_NONE, cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            2,
            function,
            assign
        ))
    );
    cleri_t * var_opt_more = cleri_sequence(
        CLERI_GID_VAR_OPT_MORE,
        2,
        var,
        cleri_optional(CLERI_NONE, cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            4,
            function,
            assign,
            instance,
            enum_
        ))
    );
    cleri_t * slice = cleri_list(CLERI_GID_SLICE, cleri_optional(CLERI_NONE, CLERI_THIS), cleri_token(CLERI_NONE, ":"), 0, 3, 0);
    cleri_t * index = cleri_repeat(CLERI_GID_INDEX, cleri_sequence(
        CLERI_NONE,
        4,
        x_index,
        slice,
        cleri_token(CLERI_NONE, "]"),
        cleri_optional(CLERI_NONE, cleri_sequence(
            CLERI_NONE,
            2,
            x_assign,
            CLERI_THIS
        ))
    ), 0, 0);
    cleri_t * end_statement = cleri_regex(CLERI_GID_END_STATEMENT, "^((;|((?s)\\/\\/.*?(\\r?\\n|$))|((?s)\\/\\*.*?\\*\\/))\\s*)*");
    cleri_t * block = cleri_sequence(
        CLERI_GID_BLOCK,
        4,
        x_block,
        comments,
        cleri_list(CLERI_NONE, CLERI_THIS, end_statement, 1, 0, 1),
        cleri_token(CLERI_NONE, "}")
    );
    cleri_t * parenthesis = cleri_sequence(
        CLERI_GID_PARENTHESIS,
        3,
        x_parenthesis,
        CLERI_THIS,
        cleri_token(CLERI_NONE, ")")
    );
    cleri_t * k_if = cleri_keyword(CLERI_GID_K_IF, "if", CLERI_CASE_SENSITIVE);
    cleri_t * k_else = cleri_keyword(CLERI_GID_K_ELSE, "else", CLERI_CASE_SENSITIVE);
    cleri_t * k_return = cleri_keyword(CLERI_GID_K_RETURN, "return", CLERI_CASE_SENSITIVE);
    cleri_t * k_for = cleri_keyword(CLERI_GID_K_FOR, "for", CLERI_CASE_SENSITIVE);
    cleri_t * k_in = cleri_keyword(CLERI_GID_K_IN, "in", CLERI_CASE_SENSITIVE);
    cleri_t * k_continue = cleri_keyword(CLERI_GID_K_CONTINUE, "continue", CLERI_CASE_SENSITIVE);
    cleri_t * k_break = cleri_keyword(CLERI_GID_K_BREAK, "break", CLERI_CASE_SENSITIVE);
    cleri_t * if_statement = cleri_sequence(
        CLERI_GID_IF_STATEMENT,
        6,
        k_if,
        cleri_token(CLERI_NONE, "("),
        CLERI_THIS,
        cleri_token(CLERI_NONE, ")"),
        CLERI_THIS,
        cleri_optional(CLERI_NONE, cleri_sequence(
            CLERI_NONE,
            2,
            k_else,
            CLERI_THIS
        ))
    );
    cleri_t * return_statement = cleri_sequence(
        CLERI_GID_RETURN_STATEMENT,
        3,
        k_return,
        CLERI_THIS,
        cleri_optional(CLERI_NONE, cleri_sequence(
            CLERI_NONE,
            2,
            cleri_token(CLERI_NONE, ","),
            CLERI_THIS
        ))
    );
    cleri_t * for_statement = cleri_sequence(
        CLERI_GID_FOR_STATEMENT,
        7,
        k_for,
        cleri_token(CLERI_NONE, "("),
        cleri_list(CLERI_NONE, var, cleri_token(CLERI_NONE, ","), 1, 0, 0),
        k_in,
        CLERI_THIS,
        cleri_token(CLERI_NONE, ")"),
        CLERI_THIS
    );
    cleri_t * expression = cleri_sequence(
        CLERI_GID_EXPRESSION,
        4,
        x_preopr,
        cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            13,
            chain,
            t_false,
            t_nil,
            t_true,
            t_float,
            t_int,
            t_string,
            t_regex,
            template,
            var_opt_more,
            thing,
            array,
            parenthesis
        ),
        index,
        cleri_optional(CLERI_NONE, chain)
    );
    cleri_t * statement = cleri_prio(
        CLERI_GID_STATEMENT,
        4,
        k_continue,
        k_break,
        cleri_choice(
            CLERI_NONE,
            CLERI_FIRST_MATCH,
            6,
            if_statement,
            return_statement,
            for_statement,
            closure,
            expression,
            block
        ),
        operations
    );
    cleri_t * statements = cleri_list(CLERI_GID_STATEMENTS, statement, end_statement, 0, 0, 1);
    cleri_t * START = cleri_sequence(
        CLERI_GID_START,
        2,
        comments,
        statements
    );
    cleri_ref_set(chain, cleri_sequence(
        CLERI_GID_CHAIN,
        4,
        x_chain,
        name_opt_more,
        index,
        cleri_optional(CLERI_NONE, chain)
    ));

    cleri_grammar_t * grammar = cleri_grammar(START, "^[A-Za-z_][0-9A-Za-z_]{0,254}(?![0-9A-Za-z_])");

    return grammar;
}

static char * query = \
            "for (x in range(100)) { \n"
            "   user = .users.find(|user| user.name.upper() == 'test');\n"
            "   \n"
            "   if (user) { \n"
            "       return 'user found!'; \n"
            "   } else { \n"
            "       wse({ \n"
            "           user = User{name: 'test'}; \n"
            "           .users.push(user); \n"
            "       }); \n"
            "       .room.emit('new-user', user); \n"
            "   } \n"
            "   \n"
            "   .channel = Channel(id); \n"
            "    .channel.user = user; \n"
            "    .users.reduce(|arr, user| { \n"
            "        user = user.filter(|prop| prop == 'name'); \n"
            "        arr.push(user); \n"
            "        arr; \n"
            "    }, []); \n"
            "}  \n";

static int test_thingsdb_lang(void)
{
    test_start("thingsdb (language)");

    cleri_grammar_t * grammar = compile_langdef();
    _assert (grammar);

    int i, flags;
    char buf[262144];
    char * str = buf;
    size_t query_len = strlen(query);
    for (i = 0; i < 20; i++)  // max 200
    {
        memcpy(str, query, query_len);
        str += query_len;
    }
    *str = '\0';

    _assert_is_valid (grammar, "x = 1");
    _assert_is_valid (grammar, "||1?2:3");
    _assert_is_valid (grammar, "||nil");
    _assert_is_valid (grammar, "4 + 5;");
    _assert_is_not_valid (grammar, "||1?2");

    flags = (
        CLERI_FLAG_EXPECTING_DISABLED|
        CLERI_FLAG_EXCLUDE_OPTIONAL|
        CLERI_FLAG_EXCLUDE_FM_CHOICE|
        CLERI_FLAG_EXCLUDE_RULE_THIS
    );

    _assert_is_valid_flags (grammar, buf, flags);

    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_thingsdb_lang() ||
        0
    );
}