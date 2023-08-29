#include <stdio.h>
#include <stdlib.h>

//目标对象，即现在业务需要的对象接口
typedef struct TypeCDisk
{
    void (*use_typeC_disk)(void* obj, int len);
    void (*free)(void* obj);
}TypeCDisk;

//已经存在的老业务模块
typedef struct USBDisk
{
    void (*use_usb_disk)(void* obj, char *color, char *texture, int len);
    void (*free)(void *obj);
    char color[32];   //颜色
    char texture[32]; //材质
    int wire_len;     //线长
}USBDisk;

static void use_usb_disk(void* obj, char *color, char *texture, int len)
{
    if(!obj || !color || !texture) return;
    USBDisk* usb = (USBDisk*)obj;
    sprintf(usb->color, "%s", color);
    sprintf(usb->texture, "%s", texture);
    usb->wire_len = len;

    printf("材质:%s 颜色:%s 长度:%dCM USB接口U盘\n",usb->texture,usb->color,usb->wire_len);
}

static void free_usb_disk(void *obj)
{
    if(obj) free(obj);
    obj=NULL;
}

USBDisk* constructor_usb_disk(void)
{
    USBDisk* usb = (USBDisk*)malloc(sizeof(USBDisk));
    usb->use_usb_disk = use_usb_disk;
    usb->free = free_usb_disk;
    return usb;
}

//-------------------------------------------------------
typedef struct AdapterDisk
{
    void (*use_typeC_disk)(void* obj, int len);
    void (*free)(void* obj);
    int wire_len;
    USBDisk* usb;
}AdapterDisk;

static void adapter_use_typeC_disk(void* obj, int len)
{
    AdapterDisk* disk = (AdapterDisk*)obj;
    disk->wire_len = len;
    disk->usb->use_usb_disk(disk->usb,"黑色","铝制",len);
}

static void adapter_use_typeC_free(void* obj)
{
    if(obj) free(obj);
    obj=NULL;
}

AdapterDisk* constructor_adapter_disk(USBDisk* usbdisk)
{
    AdapterDisk* disk = (AdapterDisk*)malloc(sizeof(AdapterDisk));
    disk->use_typeC_disk = adapter_use_typeC_disk;
    disk->free = adapter_use_typeC_free;
    disk->usb = usbdisk;
    return disk;
}

int main(void)
{
    TypeCDisk* typeC=NULL;
    printf("1.创建一个USB接口U盘。\n");
    USBDisk* usb = constructor_usb_disk();
    printf("2.连接适配器。\n");
    typeC = (TypeCDisk*)constructor_adapter_disk(usb);
    printf("3.已有Type-C接口，接上手机: ");
    typeC->use_typeC_disk(typeC, 20);

    typeC->free(typeC);
    usb->free(usb);
    return 0;
}

