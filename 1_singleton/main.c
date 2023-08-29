#include "singleton.h"
#include <pthread.h>

void* task1(void *args)
{
    int i=0;

    for(i=0; i<100; i++){
        log_get_inst()->debug("task1 i=%d....\n", i);
        log_get_inst()->warning("task1 i=%d....\n", i);
        log_get_inst()->error("task1 i=%d....\n", i);
        usleep(500000);
    }
}

void* task2(void *args)
{
    int i=0;

    for(i=0; i<100; i++){
        log_get_inst()->debug("task2 i=%d....\n", i);
        log_get_inst()->warning("task2 i=%d....\n", i);
        log_get_inst()->error("task2 i=%d....\n", i);
        usleep(500000);
    }
}

int main(int argc, char **argv)
{
    pthread_t th1, th2;

    pthread_create(&th1, NULL, task1, NULL);
    pthread_create(&th2, NULL, task2, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    log_get_inst()->destroy();
    
    return 0;
}