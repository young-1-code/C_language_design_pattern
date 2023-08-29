#include "fsm.h"

void idle_func(Event *event, void *args)
{
    int op=0;
    printf("手机处于空闲状态中...");
    printf("【输入事件: 1.响铃 2.拨号 3.空闲】: ");
    while(1){
        scanf("%d", &op);
        if(op<=0 || op > 3){
            printf("输入有误, 请重新输入: ");
        }else break;
    }
    printf("\n");
    switch (op) 
    {
    case 1: *event = E_BELL; break;
    case 2: *event = E_DIAL; break;
    case 3: *event = E_IDLE; break;
    }
}

void bell_func(Event *event, void *args)
{
    int op=0;
    printf("手机响铃中...");

    printf("【输入事件操作: 1.挂断(黑名单) 2.接通(白名单) 3.超时】: ");
    while(1){
        scanf("%d", &op);
        if(op <= 0 || op > 3){
            printf("输入有误, 重新输入: ");
        }else{
            break;
        }
    }
    printf("\n");
    switch (op)
    {
    case 1: *event = E_BLACK_LIST; break;
    case 2: *event = E_WHITE_LIST; break;
    case 3: *event = E_TIME_OUT; break;
    }
}

void talk_func(Event *event, void *args)
{
    int ret=0;
    printf("通话中... ");
    printf("【输入1-9, 通话结束】:");
    scanf("%d", &ret);
  
    *event = E_FINISH;
}

void hangup_func(Event *event, void *args)
{
    printf("\n挂断电话...\n\n");
    *event = E_IDLE;
}

void dial_func(Event *event, void *args)
{
    int op=0;
    printf("拨号中... ");
    printf("【输入事件: 1.对方忙、占线  2.接通  3.超时】: ");
    while(1){
        scanf("%d", &op);
        if(op<=0 || op > 3){
            printf("输入有误, 请重新输入: ");
        }else break;
    }
    printf("\n");
    switch (op) 
    {
    case 1: *event = E_BUSY; break;
    case 2: *event = E_CONNECT; break;
    case 3: *event = E_TIME_OUT; break;
    }
}

void timeout_func(Event *event, void *args)
{
    printf("\n超时中... \n\n");
    *event = E_IDLE;
}

FsmTable fsmtb[] = {
    /* 事件         当前状态        动作        下一个状态 */
    { E_IDLE,       S_IDLE,     idle_func,      S_IDLE },
    { E_BELL,       S_IDLE,     bell_func,      S_BELL }, 
    { E_DIAL,       S_IDLE,     dial_func,      S_DIAL },
    { E_TIME_OUT,   S_DIAL,     timeout_func,   S_TIMEOUT },
    { E_TIME_OUT,   S_BELL,     timeout_func,   S_TIMEOUT },
    { E_BUSY,       S_DIAL,     hangup_func,    S_HANGUP },
    { E_CONNECT,    S_DIAL,     talk_func,      S_TALK },
    { E_WHITE_LIST, S_BELL,     talk_func,      S_TALK }, 
    { E_BLACK_LIST, S_BELL,     hangup_func,    S_HANGUP },
    { E_FINISH,     S_TALK,     hangup_func,    S_HANGUP },
    { E_IDLE,       S_HANGUP,   idle_func,      S_IDLE },
    { E_IDLE,       S_TIMEOUT,  idle_func,      S_IDLE  }
};

/**
 * @总结:
 * 1.有多少个状态就有多少个处理函数
 * 2.状态图中有多少个事件连接线，表中就有多少项处理
 * 3.事件的作用是把状态从当前态转换到下一个状态
 * 4.发生事件后，执行的是下一个状态动作函数，即箭头所指的状态
 * 例如:发生响铃事件，执行响铃态的动作函数
*/

int main(void)
{
    int num = sizeof(fsmtb)/sizeof(fsmtb[0]);
    FSM *fsm = create_fsm(fsmtb, S_IDLE, E_IDLE, num);

    while(1)
    {
        run_fsm_action(fsm, NULL);
    }
    
    return 0;
}