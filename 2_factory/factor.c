#include <stdio.h>
#include <stdlib.h>

//定义的抽象的水果接口
typedef struct Ifruit_t
{
    void (*show)(void* obj);         //显示信息
    void (*eat)(void* obj);          //怎么操作才能吃
    void *obj;                        //指向当前的结构体
}Ifruit_t;

//------------------------------------------------
//实现香蕉相关的定义
//------------------------------------------------
typedef struct banana_t
{
    void (*show)(void* obj);         //显示信息
    void (*eat)(void* obj);     //怎么操作才能吃
    void *obj;                       //指向当前的结构体
}banana_t;

static void banana_show(void* obj)   //使用static修饰，避免被外部直接调用
{
    printf("我是香蕉!\n");
}

static void banana_eat(void* obj)  //使用static修饰，避免被外部直接调用
{
    printf("操作: 先剥掉皮，再吃!\n");
}
//香蕉的构造函数
banana_t* constructor_banana(void)  //不使用static修饰，让外部直接调用
{
    banana_t* obj = (banana_t*)malloc(sizeof(banana_t));
    obj->show = banana_show;        //给指针函数赋值，后面才能被调用
    obj->eat = banana_eat;//给指针函数赋值，后面才能被调用
    obj->obj = obj;                 //obj指向当前结构体指针
    return obj;
}

//------------------------------------------------
//实现苹果的定义
//------------------------------------------------
typedef struct apple_t
{
    void (*show)(void* obj);        //显示信息
    void (*eat)(void* obj);         //怎么吃
    void *obj;                      //指向当前的结构体
}apple_t;

static void apple_show(void* obj)   //使用static修饰，避免被外部直接调用
{
    printf("我是苹果！\n");
}

static void apple_eat(void* obj)  //使用static修饰，避免被外部直接调用
{
    printf("操作: 先水写、削皮，再吃!\n");
}
//香蕉的构造函数
apple_t* constructor_apple(void)    //不使用static修饰，让外部直接调用
{
    apple_t* obj = (apple_t*)malloc(sizeof(apple_t));
    obj->show = apple_show;         //给指针函数赋值，后面才能被调用
    obj->eat = apple_eat;           //给指针函数赋值，后面才能被调用
    obj->obj = obj;                //obj指向当前结构体指针
    return obj;
}


//------------------------------------------------
//实现苹果的定义
//------------------------------------------------
typedef struct pear_t
{
    void (*show)(void* obj);       //显示信息
    void (*eat)(void* obj);        //怎么操作才能吃
    void *obj;                     //指向当前的结构体
}pear_t;

static void pear_show(void* obj)   //使用static修饰，避免被外部直接调用
{
    printf("我是梨!\n");
}

static void pear_eat(void* obj)  //使用static修饰，避免被外部直接调用
{
    printf("操作: 先水写、削皮，加冰糖，煮着吃！\n");
}
//香蕉的构造函数
pear_t* constructor_pear(void)  //不使用static修饰，让外部直接调用
{
    pear_t* obj = (pear_t*)malloc(sizeof(pear_t));
    obj->show = pear_show;        //给指针函数赋值，后面才能被调用
    obj->eat = pear_eat;//给指针函数赋值，后面才能被调用
    obj->obj = obj;                //obj指向当前结构体指针
    return obj;
}
//----------------------------------------------------------------

enum FruitType
{
    APPLE,
    BANANA,
    PEAR,
};

Ifruit_t* factor_create_fruit(enum FruitType type) //工厂：生成水果的
{
    Ifruit_t *fruit=NULL;

    switch (type)
    {
    case APPLE:
        fruit = (Ifruit_t *)constructor_apple();
        printf("工厂: 生产苹果!\n");
        break;
    case BANANA:
        fruit = (Ifruit_t *)constructor_banana();
        printf("工厂: 生产香蕉!\n");
        break;
    case PEAR:
        fruit = (Ifruit_t *)constructor_pear();
        printf("工厂: 生产梨!\n");
        break;
    default:
        break;
    }
    return fruit;
}

int main(void)
{
    Ifruit_t *fruit=NULL;

    fruit = factor_create_fruit(APPLE); //生成苹果
    //每一次有新的水果添加进来，步骤都和下面一样的，易于扩展
    fruit->show(NULL);                  //显示苹果
    fruit->eat(NULL);                   //操作苹果
    free(fruit);                        //不使用了，释放资源
    printf("\n");

    fruit = factor_create_fruit(BANANA);
    fruit->show(NULL);
    fruit->eat(NULL);
    free(fruit);
    printf("\n");

    
    fruit = factor_create_fruit(PEAR);
    fruit->show(NULL);
    fruit->eat(NULL);
    free(fruit);

    return 0;
}