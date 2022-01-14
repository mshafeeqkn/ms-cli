#include<stdio.h>
#include <stdarg.h>


#include "ms_log.h"

static char term_file[128] = {0};

log_level_t log_level = log_dbg;

void ms_log_load_log_file() {
    FILE *fp = fopen("/tmp/.log_file", "r");
    if(fp != NULL) {
        fgets(term_file, sizeof(term_file), fp);
        term_file[strlen(term_file)-1] = 0;

        if(fopen(term_file, "w") == NULL) {
            fprintf(stderr, "Unable to open log file: [%s]", term_file);
            memset(term_file, 0, sizeof(term_file));
        }
    } else {
        fprintf(stderr, "Log file not configured\n");
    }
}

void _ms_log(log_level_t level, char *file, int line,  const char* format, ...) {
    FILE *fp;
    va_list args;
    char f[128] = {0};

    if(term_file[0] == 0 || level > log_level)
        return;

    fp = fopen(term_file, "w");
    if(fp == NULL)
        return;

    sprintf(f, "%s:%d ", file, line);
    fprintf(fp, "%-18s|", f);
    va_start (args, format);
    vfprintf (fp, format, args);
    fprintf(fp, "\n");
    va_end (args);
    fclose(fp);
}
