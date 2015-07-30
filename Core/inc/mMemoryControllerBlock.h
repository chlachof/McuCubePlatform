#ifndef _M_MCB_
#define _M_MCB_

#ifndef MCB_SIZE
#define MCB_SIZE            (100)
#endif

#define MCB_ALIGNMENT       (4)
#define MCB_ALIGNMENT_MASK  (MCB_ALIGNMENT - 1)

struct MemoryBlockList
{
    unsigned long m_ulSize;             //内存块大小
    const char * m_pchName;             //内存块名字
    struct MemoryBlockList * m_pstNext; //next指针
};

union MemoryHeap
{
    volatile unsigned long  ulDummy;
    unsigned char           pucMCB[MCB_SIZE]
};

/*
函数功能：同malloc(size)，增加name参数，用于在shell中展示僵尸碎片，当然不使用名字传入null也可以，默认名字为"N/A"
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
函数功能：整理所有内存块，使所有标记为free的内存块，物理上合在一起
输入：无
返回：无
*/
void mDefrag (void);

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
