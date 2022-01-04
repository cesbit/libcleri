/*
 * kwmatch.h - cleri keyword match element.
 */
#ifndef CLERI_KWMATCH_H_
#define CLERI_KWMATCH_H_

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>
#include <stddef.h>
#include <inttypes.h>
#include <cleri/cleri.h>

/* typedefs */
typedef struct cleri_s cleri_t;

/* public functions */
#ifdef __cplusplus
extern "C" {
#endif

cleri_t * cleri_kwmatch(uint32_t gid);

#ifdef __cplusplus
}
#endif

#endif /* CLERI_REGEX_H_ */