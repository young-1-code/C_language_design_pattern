#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

enum MODULE
{
    TEMPERATURE=0, //温度模块标识
    HUMIDITY,      //湿度模块标识
};

typedef struct info_t
{
    int collection_state;   //采集状态0:stop 1:start
    int is_normal;          //是否正常
    enum MODULE id;         //模块标识
}info_t;


//定义的slave基类
typedef struct ISalve_t
{
    void (*recv)(struct ISalve_t *obj, info_t *info); //收到来自Master的信息
    void* (*send)(struct ISalve_t *obj); //向Master发生信息
    void (*free)(struct ISalve_t *obj); //释放内存
    struct list_head node;   //链表节点
    info_t info;
}ISalve_t;

//温度模块
typedef struct temperature_slave_t
{
    void (*recv)(struct ISalve_t *obj, info_t *info); //收到来自Master的信息
    void* (*send)(struct ISalve_t *obj); //向Master发生信息
    void (*free)(struct ISalve_t *obj); //释放内存
    struct list_head node;   //链表节点
    info_t info;
    //-----------splite---------------------
    //下面可以根据自己情况添加各种成员变量
    void (*get_temperature)(struct ISalve_t *obj);
    float temperature;
}temperature_slave_t;
temperature_slave_t* construct_temperature_slave(void);

typedef struct humidity_slave_t
{
    void (*recv)(struct ISalve_t *obj, info_t *info); //收到来自Master的信息
    void* (*send)(struct ISalve_t *obj); //向Master发生信息
    void (*free)(struct ISalve_t *obj); //释放内存
    struct list_head node;   //链表节点
    info_t info;
    //-----------splite---------------------
    //下面可以根据自己情况添加各种成员变量
    void (*get_humidity)(struct ISalve_t *obj);
    float humidity; //湿度
}humidity_slave_t;
humidity_slave_t* construct_humidity_slave(void);

//---------------------------------------------------
//Master
//---------------------------------------------------

typedef struct IMaster_t
{
    struct list_head hlist;     //缓存对象的链表
    int (*publish)(struct IMaster_t *obj, info_t *info); //向所有的salve广播信息
    int (*subscriber)(struct IMaster_t *obj);
    int (*add)(struct IMaster_t *obj, ISalve_t *salve_obj); //订阅salve的信息
    int (*remove)(struct IMaster_t *obj, ISalve_t *salve_obj); //订阅salve的信息
    void (*free)(void *obj);    //释放内存
}IMaster_t;

IMaster_t *construct_master(void);


#endif // !__OBSERVER