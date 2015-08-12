#ifndef _M_SCHEDULER_
#define _M_SCHEDULER_

typedef int (*SchedulerFunction)(void*);

struct ThreadList
{
    SchedulerFunction   m_pfFunction;       //���к���ָ��
    unsigned char       m_ucPriority;       //���ȼ���ֵԽ�����ȼ�Խ��
    const char *        m_pchName;          //�߳�����
    void *              m_pvArgument;       //���в���
    unsigned long       m_ulTimeSliceMs;    //��С���м��MS
    unsigned long       m_ulTimeRemainMs;   //ʣ��ʱ��MS
    int                 m_nReturn;          //��������ֵ
    unsigned long       m_ulRunTimeMsMax;   //�������ʱ��MS
    unsigned long       m_ulRunTimeMsMin;   //��С����ʱ��MS
    
    struct ThreadList * m_pstNextBrother;   //�ֵܽڵ�
    struct ThreadList * m_pstNextSon;       //���ӽڵ�
};

struct ThreadManager
{
    struct ThreadList * m_List;             //�߳��б�
    struct ThreadList * m_Index;            //��ǰ���е��߳�
    unsigned long       m_ulRunTimeS;       //����ʱ��S
    unsigned long       m_ulRunTimeMs;      //����ʱ��MS
};

/*
�������ܣ�ע��һ���߳�
���룺����ָ��(int (*fun)(void*))�����ȼ�(unsigned char)������(const char *)�����в���(void *)����С���м��(unsigned long)
���أ��ɹ�(0),�ڴ治��(-12)
*/
int mRegisterThread (SchedulerFunction pfFunction, unsigned char ucPriority, const char * pchName, void * pvArgument, unsigned long ulTimeSliceMs);

/*
�������ܣ���ʼ����
���룺��
���أ���
*/
void mRunScheduler (void);

/*
�������ܣ�1MS��ʱ�жϺ���
���룺��
���أ���
*/
void mInterruptSchedulerTimer1Ms (void);

#ifdef _M_SHELL_
/*
�������ܣ�չʾ���е��̣߳��������ֻ��shell��ʹ��
���룺��
���أ���
*/
void mListThread4Shell (void);
#endif

#endif








/* ���Դ���
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