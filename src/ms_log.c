#include<stdio.h>
#include <stdarg.h>


static const char *term_file = "/dev/pts/1";

#define   MS_DEBUG      1

void ms_log(const char* format, ...) {
#if MS_DEBUG
    FILE *fp = fopen(term_file, "w");
    va_list args;
    va_start (args, format);
    vfprintf (fp, format, args);
    fprintf(fp, "\n");
    va_end (args);
    fclose(fp);
#endif
}
