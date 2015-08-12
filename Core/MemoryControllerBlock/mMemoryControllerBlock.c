/*
chlachof@126.com
2015.7.30
内存管理模块，创建，释放，碎片整理，展示
*/
#include "stdlib.h"
#include "string.h"
#include "mMemoryControllerBlock.h"


static const char               g_pchFree[5] = "free";
static const char               g_pchNA[4] = "N/A";
static union MemoryHeap         g_unHeap;

static struct MemoryBlockList * g_pstMemoryBlock = (struct MemoryBlockList *)g_unHeap.m_pucMCB;

/*
函数功能：同malloc(size)，增加name参数，用于在shell中展示僵尸碎片，当然不使用名字传入null也可以，默认名字为"N/A"，注意：不要取名为"free"，以免混淆
输入：内存块名字(char *)，内存块大小(unsigned long)
返回：内存块地址(void *)
*/
void * mMalloc (char * pchName, unsigned long ulSize)
{
    struct MemoryBlockList * pstMemoryTemp = NULL;
    struct MemoryBlockList * pstMemoryInsert = NULL;
    
    if(ulSize <= 0 || ulSize > MCB_SIZE) 
    {
        return NULL;
    }
    
    if(ulSize & MCB_ALIGNMENT_MASK)
    {
        ulSize += (MCB_ALIGNMENT - (ulSize & MCB_ALIGNMENT_MASK));
    }
    
    /*
    初始化内存块头节点
    */
    if(g_pstMemoryBlock->m_pchName == NULL)
    {
        if(MCB_SIZE < sizeof(struct MemoryBlockList))
        {
            return NULL;
        }
        memset(g_pstMemoryBlock, 0, MCB_SIZE);
        g_pstMemoryBlock->m_pchName = g_pchFree;
        g_pstMemoryBlock->m_ulSize = MCB_SIZE - sizeof(struct MemoryBlockList);
        g_pstMemoryBlock->m_pstNext = NULL;
    }
    
    /*
    为新创建的内存块分配合适的碎片空间，是所有比内存块大中最小的
    */
    pstMemoryTemp = g_pstMemoryBlock;
    
    while(pstMemoryTemp != NULL)
    {
        if(pstMemoryTemp->m_pchName == g_pchFree)
        {
            if(pstMemoryTemp->m_ulSize >= ulSize)
            {
                if(pstMemoryInsert == NULL)
                {
                    pstMemoryInsert = pstMemoryTemp;
                }
                else 
                {
                    if(pstMemoryInsert->m_ulSize > pstMemoryTemp->m_ulSize)
                    {
                        pstMemoryInsert = pstMemoryTemp;
                    }
                }
            }
        }
        pstMemoryTemp = pstMemoryTemp->m_pstNext;
    }
    
    if(pstMemoryInsert == NULL)
    {
        return NULL;
    }
    
    /*
    空闲大于2倍的结构体大小则断链分割成两个内存块，否则直接占有这个内存块
    */
    if(pstMemoryInsert->m_ulSize > (ulSize + 2*sizeof(struct MemoryBlockList)))
    {
        pstMemoryTemp = (struct MemoryBlockList *)((unsigned long *)(pstMemoryInsert) + ((pstMemoryInsert->m_ulSize - ulSize) >> MCB_ALIGNMENT_DIVBIT));
        pstMemoryTemp->m_ulSize = ulSize;
        pstMemoryTemp->m_pstNext = pstMemoryInsert->m_pstNext;
        
        pstMemoryInsert->m_ulSize -= (ulSize + sizeof(struct MemoryBlockList));
        pstMemoryInsert->m_pstNext = pstMemoryTemp;
    }
    else 
    {
        pstMemoryTemp = pstMemoryInsert;
    }
    
    if(pchName)
    {
        pstMemoryTemp->m_pchName = pchName;
    }
    else 
    {
        pstMemoryTemp->m_pchName = g_pchNA;
    }
    
    
    return (void *)((unsigned long *)(pstMemoryTemp) + (sizeof(struct MemoryBlockList) >> MCB_ALIGNMENT_DIVBIT));
}

/*
函数功能：同free(ptr)，释放内存块
输入：内存块地址(void *)
返回：无
*/
void mFree (void * pvAddress)
{
    struct MemoryBlockList * pstMemoryTemp = NULL;
    struct MemoryBlockList * pstMemoryTempPrev = NULL;
    struct MemoryBlockList * pstMemoryTempNext = NULL;
    
    pstMemoryTemp = g_pstMemoryBlock;
    
    while(pstMemoryTemp != NULL)
    {
        if(pstMemoryTemp->m_pchName != g_pchFree)
        {
            if(pvAddress == ((unsigned long *)(pstMemoryTemp) + (sizeof(struct MemoryBlockList) >> MCB_ALIGNMENT_DIVBIT)))
            {
                if(pstMemoryTemp->m_pstNext != NULL)
                {
                    pstMemoryTempNext = pstMemoryTemp->m_pstNext;
                }
                
                pstMemoryTemp->m_pchName = g_pchFree;
                
                /*
                前一个内存块为free
                */
                if(pstMemoryTempNext->m_pchName == g_pchFree)
                {
                    pstMemoryTemp->m_pstNext = pstMemoryTempNext->m_pstNext;
                    pstMemoryTemp->m_ulSize += pstMemoryTempNext->m_ulSize + sizeof(struct MemoryBlockList);
                }
                
                /*
                后一个内存块为free
                */
                if(pstMemoryTempPrev->m_pchName == g_pchFree)
                {
                    pstMemoryTempPrev->m_pstNext = pstMemoryTemp->m_pstNext;
                    pstMemoryTempPrev->m_ulSize += pstMemoryTemp->m_ulSize + sizeof(struct MemoryBlockList);
                }
                break;
            }
        }
        pstMemoryTempPrev = pstMemoryTemp;
        pstMemoryTemp = pstMemoryTemp->m_pstNext;
    }
}

/*
函数功能：整理所有内存块，使所有标记为free的内存块，物理上、逻辑上合在一起，由于没有MMU支持，其实碎片整理是没法实现的，因为物理地址发生了变化，而传出去的指针没法改变，故不开放接口
输入：无
返回：无
*/
void mDefrag (void)
{
    struct MemoryBlockList * pstMemoryTemp = NULL;
    struct MemoryBlockList * pstMemoryTempPrev = NULL;
    struct MemoryBlockList * pstMemoryTempNext = NULL;
    
    pstMemoryTemp = g_pstMemoryBlock;
    
    while(pstMemoryTemp != NULL)
    {
        /*
        如果是free块，则让出自己的物理地址
        */
        if(pstMemoryTemp->m_pchName == g_pchFree)
        {
            if(pstMemoryTemp->m_pstNext != NULL)
            {
                pstMemoryTempNext = pstMemoryTemp->m_pstNext;
                memcpy(pstMemoryTemp, pstMemoryTempNext, pstMemoryTempNext->m_ulSize);
            }
        }
        
        /*
        跳过中间的区域，连接物理地址
        */
        if(pstMemoryTempPrev != NULL)
        {
            if(pstMemoryTemp != (struct MemoryBlockList *)((unsigned long *)(pstMemoryTempPrev) + ((sizeof(struct MemoryBlockList) + pstMemoryTempPrev->m_ulSize) >> MCB_ALIGNMENT_DIVBIT)))
            {
                memcpy((unsigned long *)(pstMemoryTempPrev) + ((sizeof(struct MemoryBlockList) + pstMemoryTempPrev->m_ulSize) >> MCB_ALIGNMENT_DIVBIT), pstMemoryTemp, pstMemoryTemp->m_ulSize);
                pstMemoryTempPrev->m_pstNext = (struct MemoryBlockList *)((unsigned long *)(pstMemoryTempPrev) + ((sizeof(struct MemoryBlockList) + pstMemoryTempPrev->m_ulSize) >> MCB_ALIGNMENT_DIVBIT));
                pstMemoryTemp = (struct MemoryBlockList *)((unsigned long *)(pstMemoryTempPrev) + ((sizeof(struct MemoryBlockList) + pstMemoryTempPrev->m_ulSize) >> MCB_ALIGNMENT_DIVBIT));

            }
        }
        pstMemoryTempPrev = pstMemoryTemp;
        pstMemoryTemp = pstMemoryTemp->m_pstNext;
    }
}


#ifdef _M_SHELL_
/*
函数功能：展示所有的内存块，这个函数只在shell中使用
输入：无
返回：无
*/
void mListBlock4Shell (void)
{
}

/*
函数功能：使用名字来释放内存块，这个函数只在shell中使用
输入：内存块名字(char *)
返回：无
*/
void mFree4Shell (char * pchName)
{
}

#endif
