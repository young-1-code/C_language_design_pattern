/**
 * Copyright (C) 2023 Linux编程用C
 * 完全享有此软件的著作权
 *
 * @author Linux编程用C
 * @time 2023/8/27 
 * @description C语言实现访问者设计模式
 */

#ifndef __VISITOR_H__
#define __VISITOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct Info_t
{
    char  name[32]; //名字
    float price;    //一件物品的价格
    float weight;   //一件物品的重量
    int   count;    //数量
}Info_t;

struct IElement_t; 

/**
 * @brief：定义访问者抽象接口,具体访问者“继承”该定义
*/
typedef struct IVisitor_t
{
    void (*visite)(struct IVisitor_t* obj, struct IElement_t* elem);
    void (*free)(struct IVisitor_t *obj);
}IVisitor_t;

//具体实现
//计算价格
typedef struct VisitorPrice
{
    void (*visite)(struct VisitorPrice* obj, struct IElement_t* elem);
    void (*free)(struct VisitorPrice *obj);
    float discount;
    char name[32]; //活动名字
}VisitorPrice;
VisitorPrice* construct_VisitorPrice(float discount, char* name); //构造函数

//根据重量计算运费访问者
typedef struct VisitorFreight
{
    void (*visite)(struct VisitorFreight* obj, struct IElement_t* elem);
    void (*free)(struct VisitorFreight *obj);
    float freight;   //邮费
    float unit_price; //每千克邮费价格
    char name[32]; //快递名字
}VisitorFreight;
VisitorFreight* construct_VisitorFreight(float unit_price, char *name); //构造函数

//---------------------------------------------------------------------------
//      element
//---------------------------------------------------------------------------
/**
 * @brief: 定义元素(被访问者)接口，具体元素(被访问者)继承该定义
*/
typedef struct IElement_t
{
    Info_t info;
    void (*accept)(struct IElement_t* obj, struct IVisitor_t* visitor);
    void (*free)(struct IElement_t* obj);
    struct list_head node;
}IElement_t;

//货物
typedef struct GoodsElem_t
{
    Info_t info;
    void (*accept)(struct GoodsElem_t* obj, struct IVisitor_t* visitor);
    void (*free)(struct GoodsElem_t* obj);
    struct list_head node;
}GoodsElem_t;
GoodsElem_t* construct_goodselem(Info_t *info);


//购物车 管理所有的element
typedef struct ShoppingCart_t
{
    void (*accept)(struct ShoppingCart_t* obj, struct IVisitor_t* visitor);
    void (*add_elem)(struct ShoppingCart_t* obj, struct IElement_t* elem);
    void (*remove_elem)(struct ShoppingCart_t* obj, struct IElement_t* elem);
    void (*free)(struct ShoppingCart_t* obj);
    struct list_head hlist;
}ShoppingCart_t;
ShoppingCart_t* construct_shoppingcart();

#endif // !__VISITOR_H__