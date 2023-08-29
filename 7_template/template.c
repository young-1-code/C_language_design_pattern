#include <stdio.h>
#include <stdlib.h>

typedef struct Interface_t
{
    /*初始化外设USB、SPI、IIC等*/
    void (*init_peripheral)(void *obj); 
    
    /*初始化硬盘*/
    void (*init_disk)(void* obj);
    
    /*初始化内存*/
    void (*init_memory)(void* obj);
    
    /*初始化网络*/
    void (*init_net)(void *obj);
    
    /*对整个流程初始化*/
    void (*init)(void *obj);
}Interface_t;

typedef struct AMDCpuStart_t
{
    void (*init_peripheral)(void *obj);
    void (*init_disk)(void* obj);
    void (*init_memory)(void* obj);
    void (*init_net)(void *obj);
    void (*init)(void *obj);
}AMDCpuStart_t;

static void ADM_init_peripheral(void *obj)
{   
    printf("初始化AMD 外设...\n");
}
static void ADM_init_disk(void *obj)
{   
    printf("初始化AMD 硬盘...\n");
}
static void ADM_init_memory(void *obj)
{   
    printf("初始化AMD 内存...\n");
}
static void ADM_init_net(void *obj)
{   
    printf("初始化AMD 网络...\n");
}
static void ADM_init(void *obj)
{   
    AMDCpuStart_t *amd = (AMDCpuStart_t*)obj;
    printf("AMD CPU 上电初始化流程: \n");
    amd->init_peripheral(amd);
    amd->init_disk(amd);
    amd->init_memory(amd);
    amd->init_net(amd);
}   

AMDCpuStart_t* construct_amd_cpu(void)
{
    AMDCpuStart_t* obj = (AMDCpuStart_t*)malloc(sizeof(AMDCpuStart_t));
    obj->init = ADM_init;
    obj->init_disk = ADM_init_disk;
    obj->init_memory = ADM_init_memory;
    obj->init_net = ADM_init_net;
    obj->init_peripheral = ADM_init_peripheral;
    
    return obj;
}


typedef struct IntelCpuStart_t
{
    void (*init_peripheral)(void *obj);
    void (*init_disk)(void* obj);
    void (*init_memory)(void* obj);
    void (*init_net)(void *obj);
    void (*init)(void *obj);
}IntelCpuStart_t;

static void Intel_init_peripheral(void *obj)
{   
    printf("初始化Inter 外设...\n");
}
static void Intel_init_disk(void *obj)
{   
    printf("初始化Inter 硬盘...\n");
}
static void Intel_init_memory(void *obj)
{   
    printf("初始化Inter 内存...\n");
}
static void Intel_init_net(void *obj)
{   
    printf("初始化Inter 网络...\n");
}
static void Intel_init(void *obj)
{   
    IntelCpuStart_t *intel = (IntelCpuStart_t*)obj;
    printf("AMD CPU 上电初始化流程: \n");
    intel->init_peripheral(intel);
    intel->init_disk(intel);
    intel->init_memory(intel);
    intel->init_net(intel);
}   

IntelCpuStart_t* construct_intel_cpu(void)
{
    IntelCpuStart_t* obj = (IntelCpuStart_t*)malloc(sizeof(IntelCpuStart_t));
    obj->init = Intel_init;
    obj->init_disk = Intel_init_disk;
    obj->init_memory = Intel_init_memory;
    obj->init_net = Intel_init_net;
    obj->init_peripheral = Intel_init_peripheral;
    
    return obj;
}

void ADM_init_peripheral_v1(void *obj)
{
    printf("新接口: 初始化ADM外设: USB3.0、SPI、IIC接口...\n");
}

void ADM_init_disk_V1(void *obj)
{   
    printf("新接口: 初始化AMD硬盘: 三星固态硬盘(512G)、东芝固态硬盘(128G)...\n");
}
int main(void)
{   
    Interface_t *cpu=NULL;
    cpu = (Interface_t*)construct_amd_cpu();
    cpu->init(cpu);

    printf("\n二次重写的硬盘和外设初始化接口:\n");
    cpu->init_peripheral = ADM_init_peripheral_v1;
    cpu->init_disk = ADM_init_disk_V1;
    cpu->init(cpu);
    free(cpu);
    cpu = NULL;

    printf("\n");
    cpu = (Interface_t*)construct_intel_cpu();
    cpu->init(cpu);
    free(cpu);
    cpu = NULL;

    return 0;
}