/*
 * kwcache.h - holds keyword regular expression result while parsing.
 */
#ifndef CLERI_KWCACHE_H_
#define CLERI_KWCACHE_H_

#include <sys/types.h>
#include <cleri/cleri.h>
#include <cleri/parse.h>

/* typedefs */
typedef struct cleri_parse_s cleri_parse_t;
typedef struct cleri_kwcache_s cleri_kwcache_t;

/* private functions */
cleri_kwcache_t * cleri__kwcache_new(void);
ssize_t cleri__kwcache_match(cleri_parse_t * pr, const char * str);
void cleri__kwcache_free(cleri_kwcache_t * kwcache);

/* structs */
struct cleri_kwcache_s
{
    size_t len;
    const char * str;
    cleri_kwcache_t * next;
};

#endif /* CLERI_KWCACHE_H_ */

