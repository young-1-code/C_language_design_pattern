#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct IProxy_t
{
    void (*sale)(void *obj);
    void (*discount)(void *obj, float discount_num);
    void (*free)(void *obj);
}IProxy_t;

//实体店卖服装
typedef struct ClotheStore_t
{
    void (*sale)(void *obj);
    void (*discount)(void *obj, float discount_num);
    void (*free)(void *obj);
    float price;
}ClotheStore_t;

static void clothe_store_free(void *obj)
{
    ClotheStore_t *csobj = (ClotheStore_t*)obj;
    if(csobj) free(csobj);
}

static void clothe_store_sale(void *obj)
{
    ClotheStore_t *csobj = (ClotheStore_t*)obj;
    printf("服装店: 卖衣服! 价格是: %.2f元\n", ((ClotheStore_t*)obj)->price);
}

static void clothe_store_discount(void *obj, float discount_num)
{
    printf("服装店: 现在做活动打 %.1f 折, 欢迎选购! \n", discount_num);
    ClotheStore_t *cs = (ClotheStore_t*)obj;
    cs->price *= discount_num/10.0;
}

ClotheStore_t* constructor_clothe_store(void)
{
    ClotheStore_t* obj = (ClotheStore_t*)malloc(sizeof(ClotheStore_t));
    obj->price = 588;
    obj->discount = clothe_store_discount;
    obj->sale = clothe_store_sale;
    obj->free = clothe_store_free;
    return obj;
}

//------------------------------------------
//------------------------------------------
typedef struct ProxyTaoBao_t
{
    void (*sale)(void *obj);
    void (*discount)(void *obj, float discount_num);
    void (*free)(void *obj); //释放资源
    void (*before)(void *obj); //售前
    void (*after)(void *obj);  //售后
    ClotheStore_t *csobj; //给服装店做代理
}ProxyTaoBao_t;

static void taobao_sale(void *obj)
{
    ProxyTaoBao_t *tbobj = (ProxyTaoBao_t*)obj;
    tbobj->before(tbobj); //做售前工作
    tbobj->discount(tbobj, 9.5); //淘宝做活动打折9.5折

    tbobj->csobj->discount(tbobj->csobj, 9.8); //商家打折9.8折
    tbobj->csobj->sale(tbobj->csobj); //实际卖衣服还是服装商家

    tbobj->after(tbobj); //做售后工作
}

static void taobao_discount(void *obj, float discount_num)
{
    ProxyTaoBao_t *tbobj = (ProxyTaoBao_t*)obj;
    tbobj->csobj->price *= discount_num/10.0;
    printf("淘宝做活动, 全场打 %.1f 折! 欢迎选购!!!\n", discount_num);
}

static void taobao_before(void *obj)
{
    printf("淘宝售前: 亲！欢迎咨询...\n");
}

static void taobao_after(void *obj)
{
    printf("淘宝售后: 亲！有问题请联系我哦...\n");
}

static void taobao_free(void *obj)
{
    ProxyTaoBao_t* tbobj = (ProxyTaoBao_t*)obj;
    if(tbobj->csobj) free(tbobj->csobj);
    if(tbobj) free(tbobj); //释放资源
}

ProxyTaoBao_t* constructor_proxy_taobao(void)
{
    ProxyTaoBao_t* obj = (ProxyTaoBao_t*)malloc(sizeof(ProxyTaoBao_t));
    obj->after = taobao_after;
    obj->before = taobao_before;
    obj->csobj = constructor_clothe_store(); //创建实体店对象
    obj->discount = taobao_discount;
    obj->free = taobao_free;
    obj->sale = taobao_sale;
    return obj;
}


int main(void)
{
    IProxy_t *obj = NULL;

    printf("------------------------------\n");
    printf("普通实体店卖衣服:\n");
    ClotheStore_t *csobj = constructor_clothe_store();
    obj = (IProxy_t*)csobj;
    obj->discount(obj, 9.8);
    obj->sale(obj);
    obj->free(obj);

    printf("\n------------------------------\n");
    printf("淘宝代理卖衣服: \n");
    ProxyTaoBao_t *tbobj = constructor_proxy_taobao();
    obj = (IProxy_t*)tbobj;
    obj->sale(obj);
    obj->free(obj);

    return 0;
}