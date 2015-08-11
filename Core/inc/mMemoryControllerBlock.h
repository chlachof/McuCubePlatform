#ifndef _M_MCB_
#define _M_MCB_

#ifndef MCB_SIZE
#define MCB_SIZE                (1024*10)
#endif

#define MCB_ALIGNMENT           (4)
#define MCB_ALIGNMENT_MASK      (3)
#define MCB_ALIGNMENT_DIVBIT    (2)

struct MemoryBlockList
{
    unsigned long   m_ulSize;           //�ڴ���С
    const char *    m_pchName;          //�ڴ������
    struct MemoryBlockList * m_pstNext; //nextָ��
};

union MemoryHeap
{
    volatile unsigned long  ulDummy;
    unsigned char           pucMCB[MCB_SIZE];
};

/*
�������ܣ�ͬmalloc(size)������name������������shell��չʾ��ʬ��Ƭ����Ȼ��ʹ�����ִ���nullҲ���ԣ�Ĭ������Ϊ"N/A"��ע�⣺��Ҫȡ��Ϊ"free"���������
���룺�ڴ������(char *)���ڴ���С(unsigned long)
���أ��ڴ���ַ(void *)
*/
void * mMalloc (char * pchName, unsigned long ulSize);

/*
�������ܣ�ͬfree(ptr)���ͷ��ڴ��
���룺�ڴ���ַ(void *)
���أ���
*/
void mFree (void * pvAddress);

/*
�������ܣ����������ڴ�飬ʹ���б��Ϊfree���ڴ�飬�����ϡ��߼��Ϻ���һ������û��MMU֧�֣���ʵ��Ƭ������û��ʵ�ֵģ���Ϊ�����ַ�����˱仯��������ȥ��ָ��û���ı䣬�ʲ����Žӿ�
���룺��
���أ���
*/
//void mDefrag (void);

#ifdef _M_SHELL_
/*
�������ܣ�չʾ���е��ڴ�飬�������ֻ��shell��ʹ��
���룺��
���أ���
*/
void mListBlock4Shell (void);

/*
�������ܣ�ʹ���������ͷ��ڴ�飬�������ֻ��shell��ʹ��
���룺�ڴ������(char *)
���أ���
*/
void mFree4Shell (char * pchName);
#endif

#endif







/* ���Դ���
#include "mMemoryControllerBlock.h"

int main()
{
    char * test1 = (char *)mMalloc("test1", 10);
    char * test2 = (char *)mMalloc("test2", 20);
    char * test3 = (char *)mMalloc("test3", 30);
    char * test4 = (char *)mMalloc("test4", 10);
    char * test5 = (char *)mMalloc("test5", 20);
    char * test6 = (char *)mMalloc("test6", 30);
    char * test7 = (char *)mMalloc("test7", 15);
    char * test8 = (char *)mMalloc("test8", 25);
    char * test9 = (char *)mMalloc("test9", 35);
    char * test10 = (char *)mMalloc("test10", 70);
    mFree(test2);
    mFree(test4);
    mFree(test5);
    mFree(test3);
    mFree(test8);
    char * test11 = (char *)mMalloc("test11", 60);
    mDefrag();
    char * test12 = (char *)mMalloc("test12", 60);
    return 0;
}
*/
