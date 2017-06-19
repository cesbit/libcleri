/*
 * version.h
 *
 *  Created on: Jun 19, 2017
 *      Author: Jeroen van der Heijden <jeroen@transceptor.technology>
 */
#ifndef CLERI_VERSION_H_
#define CLERI_VERSION_H_

#define LIBCLERI_VERSION_MAJOR 0
#define LIBCLERI_VERSION_MINOR 9
#define LIBCLERI_VERSION_PATCH 0

#define LIBCLERI_STRINGIFY(num) #num
#define LIBCLERI_VERSION_STR(major,minor,patch)    \
    LIBCLERI_STRINGIFY(major) "."                  \
    LIBCLERI_STRINGIFY(minor) "."                  \
    LIBCLERI_STRINGIFY(patch)

#define LIBCLERI_VERSION LIBCLERI_VERSION_STR(    \
        LIBCLERI_VERSION_MAJOR,                    \
        LIBCLERI_VERSION_MINOR,                    \
        LIBCLERI_VERSION_PATCH)

const char * cleri_version(void);

#endif /* CLERI_VERSION_H_ */