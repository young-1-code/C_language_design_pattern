#ifndef __FSM_H__
#define __FSM_H__

/**
 * @简介: FSM C语言实现有限状态机
 * 作者: 微信公众号【Linux编程用C】
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

typedef enum
{
    S_IDLE=0, /* 空闲 */
    S_BELL,   /* 响铃 */
    S_TALK,   /* 通话 */
    S_HANGUP, /* 挂断 */
    S_DIAL,   /* 拨号 */
    S_TIMEOUT /* 超时 */
}State; /* 当前状态 */

typedef enum
{
    E_IDLE,       /* 空闲 */
    E_BELL,       /* 铃声 */
    E_WHITE_LIST, /* 白名单 */
    E_BLACK_LIST, /* 黑名单 */
    E_FINISH,     /* 通话结束 */
    E_BUSY,       /* 占线忙 */
    E_CONNECT,    /* 接通 */
    E_DIAL,       /* 拨号 */
    E_TIME_OUT    /* 超时 */
}Event; /* 触发事件,由外部到来 */

typedef struct FsmTable
{
    Event event;                    /* 触发事件 */
    State cur_sta;                  /* 当前状态 */
    void (*event_action)(Event *event, void *);   /* 动作函数 */
    State next_sta;                 /* 跳转状态 */
}FsmTable;
 
typedef struct FSM
{
    FsmTable    *fsmtb;     /* 状态迁移表 */
    State       cur_sta;    /* 状态机当前状态 */
    Event       event;      /* 当前的事件 */
    uint8_t     sta_max_n;  /* 状态机状态迁移数量 */
}FSM;

/**
 * @brief: 创建一个FSM结构体指针
 * @fsmtb: 填充好的状态表
 * @state: 初始状态
 * @event: 初始事件
 * @num: 状态表项个数
 * @return: 返回一个FSM结构体指针
*/
FSM* create_fsm(FsmTable* fsmtb, State state, Event event, int num);

/**
 * @breif: 根据状态表处理事件
 * @fsm: 创建好的FSM结构体指针
 * @args: 传入的参数
 * @return: 1:成功
*/
int run_fsm_action(FSM* fsm, void *args);

#endif // !__FSM_H__
