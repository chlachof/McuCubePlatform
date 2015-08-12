#include "stdlib.h"
#include "mScheduler.h"
#include "mMemoryControllerBlock.h"
#include "mErrorCode.h"

static struct ThreadManager *   g_pstThreadManager = NULL;

/*
�������ܣ�ע��һ���߳�
���룺����ָ��(int (*fun)(void*))�����ȼ�(unsigned char)������(const char *)�����в���(void *)����С���м��(unsigned long)
���أ��ɹ�(0),�ڴ治��(-12)
*/
int mRegisterThread (SchedulerFunction pfFunction, unsigned char ucPriority, const char * pchName, void * pvArgument, unsigned long ulTimeSliceMs)
{
    struct ThreadList *  pstThreadNode = NULL;
    struct ThreadList *  pstThreadNodePrev = NULL;
    struct ThreadList *  pstThreadNodeInsert = NULL;
    
    /*
    �����̹߳�����
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
    ����������߳̽ڵ�
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
    ����������֦��Ѱ�Һ��ʵ�λ�ò���ڵ㣬���ȼ�Խ�ߣ�����Խ�����255�����������ȼ��ɸߵ��͵�˳������
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
�������ܣ���ʼ����
���룺��
���أ���
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
        ���������������ȼ�˳��
        */
        while(pstThreadNode != NULL)
        {
            /*
            �������飬ͬ���ȼ���Ҫ�ֻ�����
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
            ����ʱ��¼����ʱ�䣬����ֵ��������ɺ����±���
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
�������ܣ�1MS��ʱ�жϺ���
���룺��
���أ���
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
    ���������������ȼ�˳�������߳�ʣ������ʱ��-1
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
�������ܣ�չʾ���е��̣߳��������ֻ��shell��ʹ��
���룺��
���أ���
*/
void mListThread4Shell (void);
#endif
