#include <stdio.h>
#include <stdlib.h>

//策略模式抽象接口
typedef struct Istrategy
{
    int (*operate)(void* obj);
    void (*free)(void* obj);
}Istrategy;

//具体策略类实现
//使用网线连接网络
typedef struct net_cable_strategy
{
    int (*operate)(void* obj);
    void (*free)(void* obj);

    float rate;
    float len;
}net_cable_strategy;


static int net_cable_operate(void *obj)
{
    if(!obj) return -1;
    net_cable_strategy* nobj = (net_cable_strategy*)obj;

    printf("使用【网线】给电脑连接网络...\n");
    printf("网络速率: %.2fM/s\n", nobj->rate);
    printf("网线长度: %.0fm\n", nobj->len);
}

static void net_cable_free(void *obj)
{
    if(!obj) return;
    free(obj);
    obj = NULL;
}

net_cable_strategy* construct_net_cable(void)
{
    net_cable_strategy* obj = (net_cable_strategy*)malloc(sizeof(net_cable_strategy));
    if(!obj) return NULL;
    obj->operate = net_cable_operate;
    obj->free = net_cable_free;
    obj->rate = 53.34;
    obj->len = 100;
    return obj;
}

//连接WIFI提供网络
typedef struct net_wifi_strategy
{
    int (*operate)(void* obj);
    void (*free)(void* obj);

    float rate;
    char *describe;
}net_wifi_strategy;

static int net_wifi_operate(void *obj)
{
    if(!obj) return -1;
    net_wifi_strategy* nobj = (net_wifi_strategy*)obj;

    printf("使用【WIFI】给电脑连接网络...\n");
    printf("网络速率: %.2fM/s\n", nobj->rate);
    printf("描述:%s\n", nobj->describe);
}

static void net_wifi_free(void *obj)
{
    if(!obj) return;
    net_wifi_strategy* nobj = (net_wifi_strategy*)obj;
    if(nobj->describe) free(nobj->describe);
    nobj->describe = NULL;
    free(obj);
    obj = NULL;
}

net_wifi_strategy* construct_net_wifi(void)
{
    net_wifi_strategy* obj = (net_wifi_strategy*)malloc(sizeof(net_wifi_strategy));
    if(!obj) return NULL;
    obj->operate = net_wifi_operate;
    obj->free = net_wifi_free;
    obj->rate = 23.34;
    obj->describe = (char*)malloc(64);
    sprintf(obj->describe, "WIFI4, 802.11n, 频段 2.4GHz, 理论最大速率 600Mbps");
    return obj;
}

//USB 连接手机提供网络
typedef struct net_usb_strategy
{
    int (*operate)(void* obj);
    void (*free)(void* obj);

    float rate;
    char info[32];
}net_usb_strategy;

static int net_usb_operate(void *obj)
{
    if(!obj) return -1;
    net_usb_strategy* nobj = (net_usb_strategy*)obj;

    printf("使用【USB通过手机】给电脑连接网络...\n");
    printf("网络速率: %.2fM/s\n", nobj->rate);
    printf("信息: %s\n", nobj->info);

    return 0;
}

static void net_usb_free(void *obj)
{
    if(!obj) return;
    free(obj);
    obj = NULL;
}

net_usb_strategy* construct_net_usb(void)
{
    net_usb_strategy* obj = (net_usb_strategy*)malloc(sizeof(net_usb_strategy));
    if(!obj) return NULL;
    obj->operate = net_usb_operate;
    obj->free = net_usb_free;
    obj->rate = 11.56;
    sprintf(obj->info, "无线流量,随便使用!");
    return obj;
}


//笔记本电脑
typedef struct computer
{
    void (*work)(void* obj);
    void (*free)(void* obj);
    Istrategy *net_strategy;
}computer;

static void computer_work(void *obj)
{
    if(!obj) return;
    computer* cobj = (computer*)obj;

    printf("Computer Start...\n");
    cobj->net_strategy->operate( cobj->net_strategy );
    printf("Finish...\n");
}

static void computer_free(void *obj)
{
    if(!obj) return;
    computer* cobj = (computer*)obj;
    if(cobj->net_strategy)
        cobj->net_strategy->free(cobj->net_strategy); //先释放内层对象的内存
    free(cobj);
    cobj = NULL;
}

computer* construct_computer(Istrategy *net_strategy)
{
    if(!net_strategy) return NULL;

    computer* obj = (computer*)malloc(sizeof(computer));
    obj->net_strategy = net_strategy;    
    obj->work = computer_work;
    obj->free = computer_free;
}


int main(void)
{   
    printf("Test Strategy Mode...\n");

    Istrategy *net = NULL;
    computer* com_obj = NULL;

    net = (Istrategy *)construct_net_cable();
    com_obj = construct_computer(net);
    com_obj->work(com_obj);
    com_obj->free(com_obj);
    printf("-----------split-----------\n\n");

    net = (Istrategy *)construct_net_wifi();
    com_obj = construct_computer(net);
    com_obj->work(com_obj);
    com_obj->free(com_obj);
    printf("-----------split-----------\n\n");

    net = (Istrategy *)construct_net_usb();
    com_obj = construct_computer(net);
    com_obj->work(com_obj);
    com_obj->free(com_obj);
    printf("-----------split-----------\n\n");

    return 0;
}


