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
    unsigned long   m_ulSize;           //内存块大小
    const char *    m_pchName;          //内存块名字
    struct MemoryBlockList * m_pstNext; //next指针
};

union MemoryHeap
{
    volatile unsigned long  ulDummy;
    unsigned char           pucMCB[MCB_SIZE];
};

/*
函数功能：同malloc(size)，增加name参数，用于在shell中展示僵尸碎片，当然不使用名字传入null也可以，默认名字为"N/A"，注意：不要取名为"free"，以免混淆
输入：内存块名字(char *)，内存块大小(unsigned long)
返回：内存块地址(void *)
*/
void * mMalloc (char * pchName, unsigned long ulSize);

/*
函数功能：同free(ptr)，释放内存块
输入：内存块地址(void *)
返回：无
*/
void mFree (void * pvAddress);

/*
函数功能：整理所有内存块，使所有标记为free的内存块，物理上、逻辑上合在一起，由于没有MMU支持，其实碎片整理是没法实现的，因为物理地址发生了变化，而传出去的指针没法改变，故不开放接口
输入：无
返回：无
*/
//void mDefrag (void);

#ifdef _M_SHELL_
/*
函数功能：展示所有的内存块，这个函数只在shell中使用
输入：无
返回：无
*/
void mListBlock4Shell (void);

/*
函数功能：使用名字来释放内存块，这个函数只在shell中使用
输入：内存块名字(char *)
返回：无
*/
void mFree4Shell (char * pchName);
#endif

#endif







/* 测试代码
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
