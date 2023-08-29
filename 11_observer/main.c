#include "observer.h"
#include <time.h>

int main(void)
{
    int i=0;
    srand(time(NULL));

    info_t info;
    IMaster_t *master = construct_master();

    humidity_slave_t *hslave = construct_humidity_slave();
    temperature_slave_t *tslave = construct_temperature_slave();

    master->add(master, (ISalve_t*)hslave);
    master->add(master, (ISalve_t*)tslave);

    //给模块发射控制指令
    info.collection_state = 0; 
    master->publish(master, &info);

    info.collection_state = 1;
    master->publish(master, &info);

    for(i=0; i<5; ++i){
        printf("--------------------------\n");
        printf("监测各个模块: \n");
        master->subscriber(master);
    }


    master->remove(master, (ISalve_t*)tslave);
    printf("--------remove 温度模块后------------------\n");
    printf("监测各个模块: \n");
    master->subscriber(master);

    master->free(master);
    hslave->free((ISalve_t*)hslave);
    tslave->free((ISalve_t*)tslave);

    return 0;
}