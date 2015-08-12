#include "stdlib.h"
#include "mScheduler.h"
#include "mMemoryControllerBlock.h"
#include "mErrorCode.h"

static struct ThreadManager *   g_pstThreadManager = NULL;

/*
函数功能：注册一个线程
输入：函数指针(int (*fun)(void*))，优先级(unsigned char)，名称(const char *)，运行参数(void *)，最小运行间隔(unsigned long)
返回：成功(0),内存不足(-12)
*/
int mRegisterThread (SchedulerFunction pfFunction, unsigned char ucPriority, const char * pchName, void * pvArgument, unsigned long ulTimeSliceMs)
{
    struct ThreadList *  pstThreadNode = NULL;
    struct ThreadList *  pstThreadNodePrev = NULL;
    struct ThreadList *  pstThreadNodeInsert = NULL;
    
    /*
    创建线程管理者
    */
    if(g_pstThreadManager == NULL)
    {
        g_pstThreadManager = (struct ThreadManager *)mMalloc("ThreadManager", sizeof(struct ThreadManager));
        if(g_pstThreadManager == NULL)
        {
            return (-EC_NOMEM);
        }
        g_pstThreadManager->m_List  = NULL;
        g_pstThreadManager->m_Index = NULL;
        g_pstThreadManager->m_ulRunTimeS  = 0;
        g_pstThreadManager->m_ulRunTimeMs = 0;
    }
    
    /*
    创建输入的线程节点
    */
    pstThreadNodeInsert = (struct ThreadList *)mMalloc("Thread", sizeof(struct ThreadList));
    if(pstThreadNodeInsert == NULL)
    {
        return (-EC_NOMEM);
    }
    pstThreadNodeInsert->m_pfFunction = pfFunction;
    pstThreadNodeInsert->m_ucPriority = ucPriority;
    pstThreadNodeInsert->m_pchName = pchName;
    pstThreadNodeInsert->m_pvArgument = pvArgument;
    pstThreadNodeInsert->m_ulTimeSliceMs = ulTimeSliceMs;
    pstThreadNodeInsert->m_ulTimeRemainMs = 0;
    pstThreadNodeInsert->m_nReturn = 0;
    pstThreadNodeInsert->m_ulRunTimeMsMax = 0;
    pstThreadNodeInsert->m_ulRunTimeMsMin = 0xffffffff;
    
    /*
    遍历父子树枝，寻找合适的位置插入节点，优先级越高，数字越大，最大255，链表按照优先级由高到低的顺序排列
    */
    if(g_pstThreadManager->m_List == NULL)
    {
        g_pstThreadManager->m_List = pstThreadNodeInsert;
    }
    else 
    {
        pstThreadNode = g_pstThreadManager->m_List;
        
        while(pstThreadNode != NULL)
        {
            
            if(ucPriority > pstThreadNode->m_ucPriority)
            {
                if(pstThreadNodePrev == NULL)
                {
                    g_pstThreadManager->m_List = pstThreadNodeInsert;
                    pstThreadNodeInsert->m_pstNextSon = pstThreadNode;
                }
                else 
                {
                    pstThreadNodePrev->m_pstNextSon = pstThreadNodeInsert;
                    pstThreadNodeInsert->m_pstNextSon = pstThreadNode;
                }
                break;
            }
            else if(ucPriority == pstThreadNode->m_ucPriority)
            {
                pstThreadNodeInsert->m_pstNextBrother = pstThreadNode->m_pstNextBrother;
                pstThreadNode->m_pstNextBrother = pstThreadNodeInsert;
                break;
            }
            
            pstThreadNodePrev = pstThreadNode;
            pstThreadNode = pstThreadNode->m_pstNextSon;
        }
        if(pstThreadNode == NULL)
        {
            pstThreadNodePrev->m_pstNextSon = pstThreadNodeInsert;
        }
    }
    
    return (EC_OK);
}

/*
函数功能：开始调度
输入：无
返回：无
*/
void mRunScheduler (void)
{
    struct ThreadList *  pstThreadNode = NULL;
    struct ThreadList *  pstThreadNodePrev = NULL;
    struct ThreadList *  pstThreadNodePrevElder = NULL;
    struct ThreadList *  pstThreadNodeTmp = NULL;
    
    unsigned long       ulRunTime = 0;
    
    if(g_pstThreadManager == NULL)
    {
        return;
    }
    
    while(1)
    {
        pstThreadNode = g_pstThreadManager->m_List;
        pstThreadNodePrev = NULL;
        pstThreadNodePrevElder = pstThreadNode;
        
        /*
        遍历树，依照优先级顺序
        */
        while(pstThreadNode != NULL)
        {
            /*
            断链重组，同优先级的要轮换遍历
            */
            if(g_pstThreadManager->m_Index != NULL)
            {
                if(g_pstThreadManager->m_Index->m_ucPriority == pstThreadNode->m_ucPriority)
                {
                    if(g_pstThreadManager->m_Index->m_pstNextBrother != NULL)
                    {
                        pstThreadNodeTmp = pstThreadNode;
                        while(pstThreadNodeTmp->m_pstNextBrother != NULL)
                        {
                            pstThreadNodeTmp = pstThreadNodeTmp->m_pstNextBrother;
                        }
                        pstThreadNodeTmp->m_pstNextBrother = pstThreadNode;
                        
                        g_pstThreadManager->m_Index->m_pstNextBrother->m_pstNextSon = pstThreadNode->m_pstNextSon;
                        pstThreadNode->m_pstNextSon = NULL;
                        
                        if(pstThreadNodePrev != NULL)
                        {
                            pstThreadNodePrev->m_pstNextSon = g_pstThreadManager->m_Index->m_pstNextBrother;
                            pstThreadNodePrevElder = pstThreadNodePrev->m_pstNextSon;
                            pstThreadNode = pstThreadNodePrev->m_pstNextSon;
                        }
                        else
                        {
                            g_pstThreadManager->m_List = g_pstThreadManager->m_Index->m_pstNextBrother;
                            pstThreadNodePrevElder = g_pstThreadManager->m_List;
                            pstThreadNode = g_pstThreadManager->m_List;
                        }
                        g_pstThreadManager->m_Index->m_pstNextBrother = NULL;
                    }
                }
            }
            
            /*
            运行时记录运行时间，返回值，运行完成后重新遍历
            */
            if(pstThreadNode->m_ulTimeRemainMs == 0)
            {
                g_pstThreadManager->m_Index = pstThreadNode;
                ulRunTime = g_pstThreadManager->m_ulRunTimeMs;
                pstThreadNode->m_nReturn = pstThreadNode->m_pfFunction(pstThreadNode->m_pvArgument);
                ulRunTime = g_pstThreadManager->m_ulRunTimeMs - ulRunTime;
                if(ulRunTime > pstThreadNode->m_ulRunTimeMsMax)
                {
                    pstThreadNode->m_ulRunTimeMsMax = ulRunTime;
                }
                if(ulRunTime < pstThreadNode->m_ulRunTimeMsMin)
                {
                    pstThreadNode->m_ulRunTimeMsMin = ulRunTime;
                }
                pstThreadNode->m_ulTimeRemainMs = pstThreadNode->m_ulTimeSliceMs;
                
                pstThreadNode = g_pstThreadManager->m_List;
                pstThreadNodePrev = NULL;
                pstThreadNodePrevElder = pstThreadNode;
                continue;
            }
            
            if(pstThreadNode->m_pstNextBrother != NULL)
            {
                pstThreadNodePrev = pstThreadNode;
                pstThreadNode = pstThreadNode->m_pstNextBrother;
            }
            else
            {
                pstThreadNodePrev = pstThreadNodePrevElder;
                pstThreadNode = pstThreadNodePrevElder->m_pstNextSon;
                pstThreadNodePrevElder = pstThreadNode;
            }
        }
    }
}

/*
函数功能：1MS定时中断函数
输入：无
返回：无
*/
void mInterruptSchedulerTimer1Ms (void)
{
    struct ThreadList *  pstThreadNode = NULL;
    struct ThreadList *  pstThreadNodePrevElder = NULL;
    
    if(g_pstThreadManager == NULL)
    {
        return;
    }
    
    g_pstThreadManager->m_ulRunTimeMs ++;
    if(g_pstThreadManager->m_ulRunTimeMs >= 1000)
    {
        g_pstThreadManager->m_ulRunTimeMs = 0;
        g_pstThreadManager->m_ulRunTimeS ++;
    }
    
    /*
    遍历树，依照优先级顺序，所有线程剩余运行时间-1
    */
    pstThreadNode = g_pstThreadManager->m_List;
    pstThreadNodePrevElder = pstThreadNode;
    
    while(pstThreadNode != NULL)
    {
        if(pstThreadNode->m_ulTimeRemainMs != 0)
        {
            pstThreadNode->m_ulTimeRemainMs --;
        }

        if(pstThreadNode->m_pstNextBrother != NULL)
        {
            pstThreadNode = pstThreadNode->m_pstNextBrother;
        }
        else
        {
            pstThreadNode = pstThreadNodePrevElder->m_pstNextSon;
            pstThreadNodePrevElder = pstThreadNode;
        }
    }
}

#ifdef _M_SHELL_
/*
函数功能：展示所有的线程，这个函数只在shell中使用
输入：无
返回：无
*/
void mListThread4Shell (void);
#endif
