/**
 * Copyright (C) 2023 Linux编程用C
 * 完全享有此软件的著作权
 *
 * @author Linux编程用C
 * @time 2023/8/27 
 * @description C语言实现访问者设计模式
 */

#include "visitor.h"

//----------------------------------------------
//      visitor
//----------------------------------------------

//----------------------------------------------
//      打折访问者
//----------------------------------------------
/**
 * @brief: 访问者处理函数 实现打折计算价格
 * @obj: 访问者对象，传入进去方便访问内部数据
 * @elem: 被访问的元素对象指针
 * @return: none
*/
static void VisitorPrice_discount(struct VisitorPrice* obj, struct IElement_t* elem)
{
    if(!obj || !elem) return;
    float total = (elem->info.price) * (obj->discount);
    total *= elem->info.count;

    printf("活动:%s 商品:%s 单价%.2f元,折扣%.2f折,数量%d件, 总价=%.2f\n", obj->name,\
        elem->info.name, elem->info.price, obj->discount, elem->info.count, total);
}

/**
 * @brief: 释放内存
 * @obj:访问者指针
 * @return:none
*/
static void VisitorPrice_free(struct VisitorPrice *obj)
{
    if(obj) free(obj);
    obj=NULL;
}

/**
 * @brief: 打折访问者构造函数
 * @discount: 折扣力度
 * @name: 活动名字
 * @return: 访问者对象
*/
VisitorPrice* construct_VisitorPrice(float discount, char *name)
{
    VisitorPrice* obj = (VisitorPrice*)malloc(sizeof(VisitorPrice));
    if(!obj) return NULL;
    memcpy(obj->name, name, 32);
    obj->discount = discount;
    obj->visite = VisitorPrice_discount;
    obj->free = VisitorPrice_free;
    return obj;
}

//---------------------------------------------
//      计算运费的访问者
//---------------------------------------------
static void VisitorFreight_visite(struct VisitorFreight* obj, struct IElement_t* elem)
{
    if(!obj || !elem) return;
    float price = elem->info.count * elem->info.weight * obj->unit_price; //数量*重量*每千克运费单价
    printf("快递公司:%s 运费: 商品%s, 数量%d件, 单件重%.2fkg, 总重%.2fkg, 运费总价%.2f元\n",obj->name, \
        elem->info.name,elem->info.count,elem->info.weight,elem->info.count*elem->info.weight, price);
}

static void VisitorFreight_free(struct VisitorFreight *obj)
{
    if(obj) free(obj);
    obj=NULL;
}

VisitorFreight* construct_VisitorFreight(float unit_price, char* name)
{
    VisitorFreight* obj = (VisitorFreight*)malloc(sizeof(VisitorFreight));
    if(!obj) return NULL;
    memcpy(obj->name, name, 32);
    obj->unit_price = unit_price;
    obj->visite = VisitorFreight_visite;
    obj->free = VisitorFreight_free;
    return obj;
}

//----------------------------------------------
//  element 元素对象 被访问者
//----------------------------------------------

/**
 * @brief:接受访问
 * @obj:元素对象指针
 * @visitor:访问者
 * @return: none
*/
static void elem_accept(struct GoodsElem_t* obj, struct IVisitor_t* visitor)
{
    if(!obj || !visitor) return;
    visitor->visite(visitor, (IElement_t*)obj); //接受访问者访问
}

/**
 * @brief:释放内存
 * @obj:元素对象指针
 * @return: none
*/
static void elem_free(struct GoodsElem_t* obj)
{
    if(obj) free(obj);
    obj=NULL;
}

/**
 * @brief:构造函数，创建元素对象
 * @info: 货物信息
 * @return: 对象指针
*/
GoodsElem_t* construct_goodselem(Info_t *info)
{
    if(!info) return NULL;
    GoodsElem_t* obj = (GoodsElem_t*)malloc(sizeof(GoodsElem_t));
    if(!obj) return NULL;
    memcpy(&(obj->info), info, sizeof(Info_t));//赋值
    obj->accept = elem_accept;
    obj->free = elem_free;
    return obj;
}

//------------------------------------------
//购物车
//------------------------------------------
/**
 * @brief:添加元素管理起来
 * @obj:对象指针
 * @elem:元素指针
 * @return: none
*/
static void shopping_cart_add_elem(struct ShoppingCart_t* obj, struct IElement_t* elem)
{
    if(!obj || !elem) return;
    list_add_tail(&elem->node,&obj->hlist); //将element添加到链表管理
}

/**
 * @brief:删除元素
 * @obj:对象指针
 * @elem:元素指针
 * @return: none
*/
static void shopping_cart_remove_elem(struct ShoppingCart_t* obj, struct IElement_t* elem)
{
    struct list_head *pos=NULL, *n=NULL;
    IElement_t* node=NULL;

    if(!obj || !elem) return;

    list_for_each_safe(pos, n, &obj->hlist){
        node = list_entry(pos, IElement_t, node);
        if(node==elem){
            list_del(pos);
            elem->free(elem);
            break;
        }
    }
}

/**
 * @brief:内部元素对象接受访问者访问
 * @obj: 对象指针
 * @visitor:访问者对象指针
 * @return: none
*/
static void shopping_cart_accept(struct ShoppingCart_t* obj, struct IVisitor_t* visitor)
{
    struct list_head *pos=NULL;
    IElement_t* elem=NULL;

    if(!obj || !visitor) return;
    
    list_for_each(pos, &obj->hlist){
        elem = list_entry(pos, IElement_t, node);
        elem->accept(elem, visitor);
    }
}

/**
 * @brief:释放对象及内部管理元素对象
 * @obj:对象指针
 * @return: none
*/
static void shopping_cart_free(struct ShoppingCart_t* obj)
{
    struct list_head *pos=NULL, *n=NULL;
    IElement_t* node=NULL;

    if(!obj) return;

    list_for_each_safe(pos, n, &obj->hlist){
        node = list_entry(pos, IElement_t, node);
        list_del(pos);
        node->free(node);
    }

    if(obj) free(obj);
    obj=NULL;
}

/**
 * @brief:构造函数 创建管理元素对象
 * @return: 对象指针
*/
ShoppingCart_t* construct_shoppingcart()
{
    ShoppingCart_t* obj = (ShoppingCart_t*)malloc(sizeof(ShoppingCart_t));
    memset(obj, 0, sizeof(ShoppingCart_t));
    
    INIT_LIST_HEAD(&obj->hlist);
    obj->accept = shopping_cart_accept;
    obj->add_elem = shopping_cart_add_elem;
    obj->remove_elem = shopping_cart_remove_elem;
    obj->free = shopping_cart_free;

    return obj;
}
