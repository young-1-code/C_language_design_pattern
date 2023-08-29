#include "visitor.h"

int main(void)
{
    //计算价格的
    VisitorPrice* visit1 = construct_VisitorPrice(0.85, "双十一大促销");
    VisitorPrice* visit2 = construct_VisitorPrice(0.55, "元旦节清仓大甩卖");

    //计算邮费的
    VisitorFreight* visit3 = construct_VisitorFreight(4.2, "中通快递");
    VisitorFreight* visit4 = construct_VisitorFreight(3.4, "圆通快递");

    Info_t info;
    info.count = 10;
    info.price = 32.14;
    info.weight = 1.3;
    sprintf(info.name, "玉米");
    GoodsElem_t* elem1 = construct_goodselem(&info);
    
    info.count = 50;
    info.weight = 3.3;
    sprintf(info.name, "大米");
    GoodsElem_t* elem2 = construct_goodselem(&info);

    info.price = 52.02;
    info.weight = 7.3;
    sprintf(info.name, "小麦");
    GoodsElem_t* elem3 = construct_goodselem(&info);

    ShoppingCart_t* shop = construct_shoppingcart();
    shop->add_elem(shop, (IElement_t*)elem1);
    shop->add_elem(shop, (IElement_t*)elem2);
    shop->add_elem(shop, (IElement_t*)elem3); //这里可以添加很多元素

    //访问者1
    printf("访问者1: 打85折扣\n");
    shop->accept(shop, (IVisitor_t*)visit1);

    //访问者2
    printf("\n访问者2: 打55折扣\n");
    shop->accept(shop, (IVisitor_t*)visit2);

    //访问者3
    printf("\n访问者3: 使用中通快递邮费\n");
    shop->accept(shop, (IVisitor_t*)visit3);
    
    //访问者4
    printf("\n访问者4: 使用圆通快递邮费\n");
    shop->accept(shop, (IVisitor_t*)visit4);

    shop->remove_elem(shop, (IElement_t*)elem3);
    //访问者4
    printf("\n使用圆通快递邮费,删除小麦商品\n");
    shop->accept(shop, (IVisitor_t*)visit4);


    shop->free(shop);
    visit1->free(visit1);
    visit2->free(visit2);
    visit3->free(visit3);
    visit4->free(visit4);

    return 0;
}