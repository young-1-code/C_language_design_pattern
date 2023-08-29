#include <stdio.h>
#include <stdlib.h>

//定义的抽象交通工具接口
typedef struct vehicle_t
{
    void (*run)(void *obj);
    void (*free)(void *obj);
}vehicle_t;

//------------------------------------------------------------
//"继承"上面vehicle_t接口实现car
//"继承":C语言中没有继承这个说法，此处引用C++中的继承概念
//需要在car_t结构中保持与vehicle_t定义的结构与顺序实现"继承"。
//只需要保证前面的与定义接口结构与顺序一致，后面可以加入自有的特性
//下同
typedef struct car_t
{
    void (*run)(void *obj);
    void (*free)(void *obj);
    
    //car结构的特性
    vehicle_t *m_veh;
    void (*characte)(void *obj);
}car_t;

//car的运行函数
static void car_run(void *obj)
{
    if(!obj) return; //判断指针是否有效
    car_t *car = (car_t*)obj;
    if(car->m_veh) car->m_veh->run(car->m_veh); //"发生多态调用"
    car->characte(car); //调用自身的特性
}

//car释放内存
static void car_free(void *obj)
{
    if(!obj) return;
    car_t *car = (car_t*)obj;
    
    if(car->m_veh) free(car->m_veh);
    car->m_veh = NULL;

    free(car);
    car = NULL;
    printf("释放 普通车对象 内存!!!\n");
}
//实现car特有的功能
static void car_characte(void *obj)
{
    printf("功能: 可以跑!!!\n");
}

//实现car的带参"构造函数",创建一个car对象
car_t* constuctor_car(vehicle_t* veh)
{
    car_t *obj = (car_t*)malloc(sizeof(car_t));
    obj->run = car_run;
    obj->free = car_free;
    obj->characte = car_characte;
    obj->m_veh = veh;
    return obj;
}

//------------------------------------------------------------
typedef struct suvcar_t
{
    void (*run)(void *obj);
    void (*free)(void *obj);
    
    //car结构的特性
    vehicle_t *m_veh;
    void (*characte)(void *obj);
}suvcar_t;

static void suv_run(void *obj)
{
    if(!obj) return;
    suvcar_t *suv = (suvcar_t*)obj;
    
    if(suv->m_veh) suv->m_veh->run(suv->m_veh);
    suv->characte(suv);
}

static void suv_free(void *obj)
{
    if(!obj) return;
    suvcar_t *suv = (suvcar_t*)obj;
    
    if(suv->m_veh) free(suv->m_veh);
    suv->m_veh = NULL;

    free(suv);
    suv=NULL;

    printf("释放 越野车对象 内存!!!\n");
}

static void suv_characte(void *obj)
{
    printf("功能: 可以越野!!!\n");
}

suvcar_t* constuctor_suvcar(vehicle_t *veh)
{
    if(!veh) return NULL;
    suvcar_t *suv = (suvcar_t*)malloc(sizeof(suvcar_t));
    suv->run = suv_run;
    suv->free = suv_free;
    suv->characte = suv_characte;
    suv->m_veh = veh;
    return suv;
}


//------------------------------------------------------------
typedef struct racecar_t
{
    void (*run)(void *obj);
    void (*free)(void *obj);
    
    //car结构的特性
    vehicle_t *m_veh;
    void (*characte)(void *obj);
}racecar_t;

static void race_run(void *obj)
{
    if(!obj) return;
    racecar_t *race = (racecar_t*)obj;
    
    if(race->m_veh) race->m_veh->run(race->m_veh);
    race->characte(race);
}

static void race_free(void *obj)
{
    if(!obj) return;
    racecar_t *race = (racecar_t*)obj;
    
    if(race->m_veh) free(race->m_veh);
    race->m_veh = NULL;

    free(race);
    race=NULL;
    printf("释放 赛车对象 内存!!!\n");

}

static void race_characte(void *obj)
{
    printf("功能: 速度快,可以竞速!!!\n");
}

racecar_t* constuctor_racecar(vehicle_t *veh)
{
    if(!veh) return NULL;
    racecar_t *race = (racecar_t*)malloc(sizeof(racecar_t));
    race->run = race_run;
    race->free = race_free;
    race->characte = race_characte;
    race->m_veh = veh;
    return race;
}


//------------------------------------------------------------
typedef struct tourcar_t
{
    void (*run)(void *obj);
    void (*free)(void *obj);
    
    //car结构的特性
    vehicle_t *m_veh;
    void (*characte)(void *obj);
}tourcar_t;

static void tour_run(void *obj)
{
    if(!obj) return;
    tourcar_t *tour = (tourcar_t*)obj;
    
    if(tour->m_veh) tour->m_veh->run(tour->m_veh);
    tour->characte(tour);
}

static void tour_free(void *obj)
{
    if(!obj) return;
    tourcar_t *tour = (tourcar_t*)obj;
    
    if(tour->m_veh) free(tour->m_veh);
    tour->m_veh = NULL;

    free(tour);
    tour=NULL;
    printf("释放 房车对象 内存!!!\n");
}

static void tour_characte(void *obj)
{
    printf("功能: 空间大,可以做饭、睡觉、旅行!!!\n");
}

tourcar_t* constuctor_tourcar(vehicle_t *veh)
{
    if(!veh) return NULL;
    tourcar_t *tour = (tourcar_t*)malloc(sizeof(tourcar_t));
    tour->run = tour_run;
    tour->free = tour_free;
    tour->characte = tour_characte;
    tour->m_veh = veh;
    return tour;
}

int main(void)
{
    vehicle_t *veh=NULL;

    printf("\033[1;43;33mC语言实现设计模式-装饰模式!\033[0m\n");
    printf("开始装饰 \033[1;42;32m[汽车]\033[0m: \n");
    car_t *car = constuctor_car(veh);
    car->run(car); //调用汽车的功能

    printf("\n开始装饰 \033[1;42;32m[越野车]\033[0m: \n");
    //在普通汽车基础上(装饰),增加越野车功能
    suvcar_t *suv = constuctor_suvcar((vehicle_t*)car);
    suv->run(suv); //拥有普通汽车的功能，再添加上suv的功能
    
    printf("\n开始装饰 \033[1;42;32m[赛车]\033[0m: \n");
    //在越野车基础上(装饰),增加赛车功能
    racecar_t *race = constuctor_racecar((vehicle_t*)suv);
    race->run(race); //拥有越野车、普通车功能，再添加上竞速功能
    
    printf("\n开始装饰 \033[1;42;32m[超级房车]\033[0m: \n");
    //在竞速汽车基础上(装饰),增加房车功能
    tourcar_t *tour = constuctor_tourcar((vehicle_t*)race); 
    tour->run(tour); //拥有竞速车、越野车、普通车功能，再添加上房车功能
    
    //printf("\033[显示方式;字背景颜色;字体颜色m 字符串 \033[0m" );
    printf("\n\033[1;41;31m 释放资源 \033[0m \n");
    tour->free(tour);
    race->free(race);
    suv->free(suv);
    car->free(car);

    return 0;
}