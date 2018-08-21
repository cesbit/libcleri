#include "buffer.h"

buffer_t * buffer_create()
{
    size_t sz = 8; // als ik deze op 16 zet--> errors
    buffer_t * buf = malloc(sizeof(buffer_t));
    if (buf)
    {
        buf->buf = calloc(sz, sizeof(sz));
        if (!buf->buf)
        {
            free(buf);
            return NULL;
        }
        buf->n = 0;
        buf->sz = sz;
    }
    return buf;
}


int buffer_printf(buffer_t * buf, const char * fmt, ...)
{

    size_t rest_sz = buf->sz - buf->n;
    int rc;
    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    size_t count = vsnprintf(buf->buf + buf->n, rest_sz, fmt, args);
    va_end(args);

    if (count < 0)
    {
        rc = - 1;
        goto finished;
    }

    if (count >= rest_sz)
    {
        size_t new_sz = buf->sz * 2;
        if (new_sz <= buf->n + count)
        {
            new_sz = buf->n + count + 1;
        }
        char * tmp = realloc(buf->buf, new_sz);
        if (!tmp)
        {
            rc = -1;
            goto finished;
        }
        buf->buf = tmp;
        buf->sz = new_sz;
        vsnprintf(buf->buf + buf->n, buf->sz - buf->n, fmt, args_copy);
    }

    buf->n += count;
    rc = 0;

finished:
    va_end(args_copy);
    return rc;
}

void buffer_destroy(buffer_t * buf)
{
    if (buf)
    {
        free(buf->buf);
    }
    free(buf);
}
