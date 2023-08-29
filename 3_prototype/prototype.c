#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Interface_t
{
    struct Interface_t*(*clone)(void *obj);
    void (*set)(void* obj, const char* name, int age);
    void (*show)(void* obj);
    char name[32];
    int age;
}Interface_t;

typedef struct Student_t
{
    struct Interface_t*(*clone)(void *obj);
    void (*set)(void* obj, const char* str, int age);
    void (*show)(void* obj);
    char name[32];
    int age;
}Student_t;

static void student_set(void* obj, const char* str, int age)
{
    if(!obj || !str) return;
    Student_t* s = (Student_t*)obj;
    strcpy(s->name, str);
    s->age = age;
}

static void student_show(void *obj)
{
    Student_t *s = (Student_t*)obj;
    printf("显示 学生信息: 姓名[%s] 年龄[%d]\n", s->name, s->age);
}

static Interface_t* student_clone(void *obj)
{
    Interface_t *iobj = (Interface_t*)malloc(sizeof(Interface_t));
    Student_t* s = (Student_t*)obj;
    if(!iobj) return NULL;
    iobj->show = student_show;
    iobj->set = student_set;
    iobj->clone = student_clone;
    strcpy(iobj->name, s->name);
    iobj->age = s->age;

    printf("调用clone()函数, clone Student\n");

    return iobj;
}

Student_t* constructor_student(void)
{
    Student_t* s = (Student_t*)malloc(sizeof(Student_t));
    s->clone = student_clone;
    s->set = student_set;
    s->show = student_show;
    return s;
}

int main(void)
{
    printf("原始的学生信息: \n");
    Student_t *s1 = constructor_student();
    s1->set(s1, "张三", 32);
    s1->show(s1); 
    printf("\n");

    Interface_t* i1 = s1->clone(s1);
    free(s1);

    printf("调用clone(),拷贝后的信息: \n");
    i1->show(i1);
    printf("\n");


    printf("重新修改信息: ");
    i1->set(i1, "李四", 41);
    i1->show(i1);
    free(i1);


    return 0;
}