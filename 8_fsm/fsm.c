#include "fsm.h"

/**
 * @breif: 遍历状态表，处理事件
 * @fsm: 创建好的FSM结构体指针
 * @args: 传入的参数
 * @return: 1:成功
*/
int run_fsm_action(FSM* fsm, void *args)
{
    int max_n = fsm->sta_max_n, i=0;
    State cur_sta = fsm->cur_sta;
    FsmTable *fsmtb = fsm->fsmtb;
    if(!fsm) return -1;

    for(i=0; i<max_n; ++i){
        if(fsmtb[i].cur_sta == cur_sta && fsmtb[i].event == fsm->event){
            fsmtb[i].event_action(&fsm->event, args);  /* 调用对应的处理函数 */
            fsm->cur_sta = fsmtb[i].next_sta;           /* 转移到下一个状态 */
            break;
        }
    }

    return 0;
}

/**
 * @brief: 创建一个FSM结构体指针
 * @fsmtb: 填充好的状态表
 * @state: 初始状态
 * @event: 初始事件
 * @num: 状态表项个数
 * @return: 返回一个FSM结构体指针
*/
FSM* create_fsm(FsmTable* fsmtb, State state, Event event, int num)
{
    FSM* fsm = (FSM*)malloc(sizeof(FSM));
    fsm->cur_sta = state;
    fsm->event = event;
    fsm->fsmtb = fsmtb;
    fsm->sta_max_n = num;

    return fsm;
}