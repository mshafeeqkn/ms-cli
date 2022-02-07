#ifndef __MS_MEM_H__
#define __MS_MEM_H__

#define ms_alloc(size)     _ms_alloc(__FILE__, __LINE__, size);
#define ms_free(m)          _ms_free(__FILE__, __LINE__, m);


extern void *_ms_alloc(char *file, int line, size_t size);
extern void _ms_free(char *file, int line, void *ptr);

#endif
