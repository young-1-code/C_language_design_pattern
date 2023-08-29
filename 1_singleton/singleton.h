#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>       
#include <unistd.h>
#include <time.h>

#ifdef _REENTRANT //是否使用多线程
#include <pthread.h>
#endif

#define LOG_FILE_SIZE (10*1024U)
#define LOG_FILE_NAME "./log/test"

typedef struct log_t
{
    int  (*debug)(const char *__restrict __fmt, ...);
    int  (*warning)(const char *__restrict __fmt, ...);
    int  (*error)(const char *__restrict __fmt, ...);
    void (*destroy)();

    int  log_size;
    char log_name[128];
    FILE *wfile;
}log_t;

#ifdef _REENTRANT //是否使用多线程
static pthread_mutex_t log_mutex;
#endif

static log_t* singleton_log = NULL;

log_t* log_get_inst(void);

#endif // !__SINGLETON