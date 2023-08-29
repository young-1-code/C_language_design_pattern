#include <stdio.h>
#include <stdlib.h>

#define MAX_SUB_NODE (16U)

typedef struct IComposite_t
{
    int (*work)(struct IComposite_t *obj);
    int (*add)(struct IComposite_t *obj, struct IComposite_t* node);
    char info[128];
    int subnode_cnt;
    struct IComposite_t *subnode[MAX_SUB_NODE];
}IComposite_t;

static int work_something(IComposite_t *obj)
{
    if(!obj || !obj->info) return -1;
    printf("%s\n", obj->info);
    //do something
    return 0;
}

static int add_node(IComposite_t *obj, IComposite_t* node)
{
    if(!obj || !node || obj->subnode_cnt >= MAX_SUB_NODE || obj->subnode_cnt<0) return -1; //添加失败
    obj->subnode[obj->subnode_cnt] = node;
    ++(obj->subnode_cnt); //子节点加一
    return 0;
}

IComposite_t* construction_composite(char *name, char *info)
{
    IComposite_t* node = (IComposite_t*)malloc(sizeof(IComposite_t));
    if(!node) return NULL;

    node->add = add_node;
    node->work = work_something;
    snprintf(node->info, 128, "%s : %s", name, info);
    node->subnode_cnt = 0;

    return node;
}

//-----------------------------------
void order_tree(IComposite_t* root, int level)
{
    if(!root) return;
    int i=0;
    for(i=0; i<level; i++){
        printf("\t");
    }

    root->work(root); //工作
    for( i=0; i<root->subnode_cnt; ++i){
        order_tree(root->subnode[i], level+1);
    }
}

void destroy_tree(IComposite_t* root)
{
    if(!root) return;
    int i=0;
    for(i=0; i<root->subnode_cnt; ++i){
        destroy_tree(root->subnode[i]);
    }
    free(root);
    root = NULL;
}


int main(void)
{
    IComposite_t* ceo = construction_composite("CEO", "批阅文件");

    IComposite_t* manager1 = construction_composite("研发总经理", "技术架构规划设计");
    ceo->add(ceo, manager1);
    IComposite_t* department_manager1 = construction_composite("一部门经理", "项目规划");
    manager1->add(manager1, department_manager1);
    IComposite_t* coder1 = construction_composite("程序员1", "敲代码");
    department_manager1->add(department_manager1, coder1);
    IComposite_t* coder2 = construction_composite("程序员2", "敲代码");
    department_manager1->add(department_manager1, coder2);
    IComposite_t* coder3 = construction_composite("程序员3", "敲代码");
    department_manager1->add(department_manager1, coder3);

    IComposite_t* department_manager2 = construction_composite("二部门经理", "项目规划");
    manager1->add(manager1, department_manager2);
    coder1 = construction_composite("程序员1", "敲代码");
    department_manager2->add(department_manager2, coder1);
    coder2 = construction_composite("程序员2", "敲代码");
    department_manager2->add(department_manager2, coder2);
    coder3 = construction_composite("程序员3", "敲代码");
    department_manager2->add(department_manager2, coder3);

    IComposite_t* department_manager3 = construction_composite("三部门经理", "项目规划");
    manager1->add(manager1, department_manager3);
    coder1 = construction_composite("程序员1", "敲代码");
    department_manager3->add(department_manager3, coder1);
    coder2 = construction_composite("程序员2", "敲代码");
    department_manager3->add(department_manager3, coder2);
    coder3 = construction_composite("程序员3", "敲代码");
    department_manager3->add(department_manager3, coder3);


    IComposite_t* manager2 = construction_composite("销售总经理", "开拓市场");
    ceo->add(ceo, manager2);
    IComposite_t *saler1 = construction_composite("销售员1", "跑业务");
    manager2->add(manager2, saler1);
    IComposite_t *saler2 = construction_composite("销售员2", "跑业务");
    manager2->add(manager2, saler2);
    IComposite_t *saler3 = construction_composite("销售员3", "跑业务");
    manager2->add(manager2, saler3);

    order_tree(ceo, 0);

    destroy_tree(ceo);

    return 0;
}