#ifndef __MS_UTILS_H__
#define __MS_UTILS_H__
#include "ms_defines.h"

#define IS_EMPTY_STR(n)                 (n == NULL || strcmp(n, EMPTY_STR) == 0)

#define FOR_EACH_STR(str, part, size)   char *part; int size; char _tmp[2048] = {0}; \
                                        strcpy(_tmp, str); \
                                        for(part = strtok(_tmp, " "); part != NULL && (size = strlen(part)); part = strtok(NULL, " "))
#endif /* __MS_UTILS_H__ */
