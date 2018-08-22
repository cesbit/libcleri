#ifndef CLERI_BUFFER_H_
#define CLERI_BUFFER_H_

#include <stdio.h>
#include <cleri/cleri.h>
#include <stdarg.h>

#define SIZE 1024

typedef struct buffer_s buffer_t;

buffer_t * buffer_create();
int buffer_printf(buffer_t * buf, const char * fmt, ...);
void buffer_destroy(buffer_t * buf);

struct buffer_s
{
    size_t n;
    size_t sz;
    char * buf;
};

#endif /* CLERI_BUFFER_H_ */