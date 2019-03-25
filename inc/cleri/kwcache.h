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

/* private functions */
uint16_t * cleri__kwcache_new(const char * str);
ssize_t cleri__kwcache_match(cleri_parse_t * pr, const char * str);
static inline void cleri__kwcache_free(uint16_t * kwcache);


/*
 * Destroy kwcache. (parsing NULL is allowed)
 */
static inline void cleri__kwcache_free(uint16_t * kwcache)
{
    free(kwcache);
}

#endif /* CLERI_KWCACHE_H_ */

