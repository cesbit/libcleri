#ifndef CLERI_TEST_EXPECTING_H_
#define CLERI_TEST_EXPECTING_H_


#define CLERI_REGEXSTR_FLOAT "^[-+]?[0-9]*\\.?[0-9]+"
#define CLERI_REGEXSTR_INTEGER "^[-+]?[0-9]+"
#define CLERI_REGEXSTR_UINTEGER "^[0-9]+"
#define CLERI_REGEXSTR_TIME_STR "^[0-9]+[smhdw]"
#define CLERI_REGEXSTR_SINGLEQ_STR "^(?:\'(?:[^\']*)\')+"
#define CLERI_REGEXSTR_DOUBLEQ_STR "^(?:\"(?:[^\"]*)\")+"
#define CLERI_REGEXSTR_GRAVE_STR "^(?:`(?:[^`]*)`)+"
#define CLERI_REGEXSTR_REGEX "^(/[^/\\\\]*(?:\\\\.[^/\\\\]*)*/i?)"


#include <stdio.h>
#include <cleri/cleri.h>
#include "../buffer.h"


const char * get_cleri_obj(cleri_t * object, int regex_choice);
void test_autocor(cleri_grammar_t * grammar, const char * str, buffer_t * buf);

#endif /* CLERI_TEST_EXPECTING_H_ */