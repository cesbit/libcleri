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
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "buffer.h"

//const char * re_to_str(uint32_t gid);
void expecting_str(cleri_grammar_t * grammar, const char * str);

#endif /* CLERI_TEST_EXPECTING_H_ */