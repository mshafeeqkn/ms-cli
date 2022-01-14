#ifndef __MS_MEM_H__
#define __MS_MEM_H__

#define ms_malloc(size)     _ms_malloc(__FILE__, __LINE__, size);
#define ms_free(m)          _ms_free(__FILE__, __LINE__, m);


extern void *_ms_malloc(char *file, int line, size_t size);
extern void _ms_free(char *file, int line, void *ptr);

#endif
