/*
chlachof@126.com
2015.7.30
*/
#include "mMemoryControllerBlock.h"
#include "stdlib.h"

const char          g_pchFree[5] = "free";
const char          g_pchNA[4] = "N/A";
union MemoryHeap    g_unHeap;

struct MemoryBlockList * g_pstMemoryBlock = (struct MemoryBlockList *)g_unHeap.pucMCB;

/*
函数功能：同malloc(size)，增加name参数，用于在shell中展示僵尸碎片，当然不使用名字传入null也可以，默认名字为"N/A"
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
    
    if(pstMemoryInsert->m_ulSize > (ulSize + 2*sizeof(struct MemoryBlockList)))
    {
        pstMemoryTemp = (struct MemoryBlockList *)((char *)pstMemoryInsert + pstMemoryInsert->m_ulSize - ulSize);
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
    
    
    return ((void *)((char *)pstMemoryTemp + sizeof(struct MemoryBlockList)));
}

/*
函数功能：同free(ptr)，释放内存块
输入：内存块地址(void *)
返回：无
*/
void mFree (void * pvAddress)
{
    struct MemoryBlockList * pstMemoryTemp = NULL;
    struct MemoryBlockList * pstMemoryTempPrv = NULL;
    
    pstMemoryTemp = g_pstMemoryBlock;
    
    while(pstMemoryTemp != NULL)
    {
        if(pstMemoryTemp->m_pchName != g_pchFree)
        {
            if(pvAddress == ((char *)pstMemoryTemp + sizeof(struct MemoryBlockList)))
            {
                if(pstMemoryTempPrv->m_pchName == g_pchFree)
                {
                    pstMemoryTempPrv->m_pstNext = pstMemoryTemp->m_pstNext;
                    pstMemoryTempPrv->m_ulSize += pstMemoryTemp->m_ulSize + sizeof(struct MemoryBlockList);
                }
                else 
                {
                    pstMemoryTemp->m_pchName = g_pchFree;
                }
                break;
            }
        }
        pstMemoryTempPrv = pstMemoryTemp;
        pstMemoryTemp = pstMemoryTemp->m_pstNext;
    }
}

/*
函数功能：整理所有内存块，使所有标记为free的内存块，物理上、逻辑上合在一起
输入：无
返回：无
*/
void mDefrag (void)
{
    
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
