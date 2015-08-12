#ifndef _M_SCHEDULER_
#define _M_SCHEDULER_

typedef int (*SchedulerFunction)(void*);

struct ThreadList
{
    SchedulerFunction   m_pfFunction;       //运行函数指针
    unsigned char       m_ucPriority;       //优先级，值越大优先级越高
    const char *        m_pchName;          //线程名字
    void *              m_pvArgument;       //运行参数
    unsigned long       m_ulTimeSliceMs;    //最小运行间隔MS
    unsigned long       m_ulTimeRemainMs;   //剩余时间MS
    int                 m_nReturn;          //函数返回值
    unsigned long       m_ulRunTimeMsMax;   //最大运行时间MS
    unsigned long       m_ulRunTimeMsMin;   //最小运行时间MS
    
    struct ThreadList * m_pstNextBrother;   //兄弟节点
    struct ThreadList * m_pstNextSon;       //儿子节点
};

struct ThreadManager
{
    struct ThreadList * m_List;             //线程列表
    struct ThreadList * m_Index;            //当前运行的线程
    unsigned long       m_ulRunTimeS;       //运行时间S
    unsigned long       m_ulRunTimeMs;      //运行时间MS
};

/*
函数功能：注册一个线程
输入：函数指针(int (*fun)(void*))，优先级(unsigned char)，名称(const char *)，运行参数(void *)，最小运行间隔(unsigned long)
返回：成功(0),内存不足(-12)
*/
int mRegisterThread (SchedulerFunction pfFunction, unsigned char ucPriority, const char * pchName, void * pvArgument, unsigned long ulTimeSliceMs);

/*
函数功能：开始调度
输入：无
返回：无
*/
void mRunScheduler (void);

/*
函数功能：1MS定时中断函数
输入：无
返回：无
*/
void mInterruptSchedulerTimer1Ms (void);

#ifdef _M_SHELL_
/*
函数功能：展示所有的线程，这个函数只在shell中使用
输入：无
返回：无
*/
void mListThread4Shell (void);
#endif

#endif








/* 测试代码
#include "stdlib.h"
#include "mMemoryControllerBlock.h"
#include "mScheduler.h"

int test1(void * pvArg)
{
    return 1;
}

int main()
{
    mRegisterThread(test1, 1, "1", NULL, 10);
    mRegisterThread(test1, 2, "2", NULL, 10);
    mRegisterThread(test1, 2, "3", NULL, 10);
    mRegisterThread(test1, 2, "4", NULL, 10);
    mRegisterThread(test1, 2, "5", NULL, 10);
    mRegisterThread(test1, 3, "6", NULL, 10);
    mRegisterThread(test1, 3, "7", NULL, 10);
    mRunScheduler();
    return 0;
}
*/