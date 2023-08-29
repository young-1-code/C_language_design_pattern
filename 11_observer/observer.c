#include "observer.h"

//----------------------------------
//     Master
//----------------------------------
static int master_publish(IMaster_t *obj, info_t *info) 
{
    struct list_head *pos=NULL;
    ISalve_t *salve=NULL;
    if(!obj) return -1;

    list_for_each(pos, &(obj->hlist)){
        salve = list_entry(pos, ISalve_t, node);
        salve->recv(salve, info); //向slave广播信息,对应salve来说是收信息
    }

    return 0;
}

static int matser_subscriber(struct IMaster_t *obj)
{
    if(!obj) return -1;
    struct list_head *pos=NULL;
    ISalve_t *salve=NULL;
    void *info = NULL;

    list_for_each(pos, &(obj->hlist)){
        salve = list_entry(pos, ISalve_t, node);
        info = salve->send(salve); //收到来自slave的info信息
        //处理slave的信息
        //...........
        switch (((info_t*)info)->id)
        {
        case TEMPERATURE:
            printf("温度采集模块: ");
            break;
        case HUMIDITY:
            printf("湿度采集模块: ");
            break;
        default:
            break;
        }
        if(((info_t*)info)->is_normal==1){
            printf("采集数据值:正常!\n");
        }else{
            printf("采集数据值:异常!\n");
        }
    }

    return 0;
}

static int master_add_slave(IMaster_t *obj, ISalve_t *salve_obj) 
{
    if(!obj || !salve_obj) return -1; //传入参数不正确
    list_add_tail(&(salve_obj->node), &(obj->hlist)); //将salve对象加入链表
    return 0;
}

static int master_remove_salve(struct IMaster_t *obj, ISalve_t *salve_obj) //订阅salve的信息
{
    struct list_head *pos=NULL, *n=NULL;
    if(!obj || !salve_obj) return -1;
    
    list_del(&(salve_obj->node));
    
    return 0;
}

static void master_free(void *obj) 
{
    if(!obj) return;
    free(obj);
}

IMaster_t *construct_master(void)
{
    IMaster_t* obj = (IMaster_t*)malloc(sizeof(IMaster_t));
    INIT_LIST_HEAD(&obj->hlist);
    obj->add = master_add_slave;
    obj->remove = master_remove_salve;
    obj->publish = master_publish;
    obj->subscriber = matser_subscriber;
    obj->free = master_free;
    
    return obj;
}

//----------------------------------
//     Salve
//----------------------------------

//公共的函数
static void slave_free(struct ISalve_t *obj) //释放内存
{
    if(!obj) return;
    free(obj);
}

//1.温度模块
static void slave_temperature_recv(struct ISalve_t *obj, info_t *info) //收到来自Master的信息
{
    if(!obj || !info) return;
    obj->info.collection_state = info->collection_state;
    printf("温度采集模块 收到来自Mater指令: [%s] 采集数据\n", obj->info.collection_state==1?"开始":"停止");
}

static void* slave_temperature_send(struct ISalve_t *obj) //向Master发生信息
{
    temperature_slave_t* tslave = (temperature_slave_t*)obj;
    if(!obj) return NULL;
    tslave->get_temperature(obj);

    return &obj->info; //把信息返回出去
}

static void slave_get_temperature(struct ISalve_t *obj)
{
    temperature_slave_t* tslave = (temperature_slave_t*)obj;
    if(!obj) return;
    tslave->temperature = rand()%50; //取随机数
    tslave->info.is_normal = tslave->temperature > 32 ? 0 : 1;
}

temperature_slave_t* construct_temperature_slave(void)
{
    temperature_slave_t* slave = (temperature_slave_t*)malloc(sizeof(temperature_slave_t));
    slave->free = slave_free;
    slave->send = slave_temperature_send;
    slave->recv = slave_temperature_recv;
    slave->get_temperature = slave_get_temperature;
    slave->info.id = TEMPERATURE;
    return slave;
}

//2.湿度模块
static void slave_humidity_recv(struct ISalve_t *obj, info_t *info) //收到来自Master的信息
{
    if(!obj || !info) return;
    obj->info.collection_state = info->collection_state;
    printf("湿度采集模块 收到来自Mater指令: [%s] 采集数据\n", obj->info.collection_state==1?"开始":"停止");
}

static void* slave_humidity_send(struct ISalve_t *obj) //向Master发生信息
{
    humidity_slave_t* hslave = (humidity_slave_t*)obj;

    if(!obj) return NULL;
    hslave->get_humidity(obj);

    return &obj->info; //把信息返回出去
}

static void slave_get_humidity(struct ISalve_t *obj)
{
    if(!obj) return;
    humidity_slave_t* hslave = (humidity_slave_t*)obj;

    hslave->humidity = rand()%100; //取随机数
    hslave->info.is_normal = (hslave->humidity > 70) ? 0 : 1;
}

humidity_slave_t* construct_humidity_slave()
{
    humidity_slave_t* slave = (humidity_slave_t*)malloc(sizeof(humidity_slave_t));
    slave->free = slave_free;
    slave->send = slave_temperature_send;
    slave->recv = slave_humidity_recv;
    slave->get_humidity = slave_get_humidity;
    slave->info.id = HUMIDITY;

    return slave;
}
