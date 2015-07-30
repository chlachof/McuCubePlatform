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
�������ܣ�ͬmalloc(size)������name������������shell��չʾ��ʬ��Ƭ����Ȼ��ʹ�����ִ���nullҲ���ԣ�Ĭ������Ϊ"N/A"
���룺�ڴ������(char *)���ڴ���С(unsigned long)
���أ��ڴ���ַ(void *)
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
    ��ʼ���ڴ��ͷ�ڵ�
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
    Ϊ�´������ڴ�������ʵ���Ƭ�ռ䣬�����б��ڴ�������С��
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
�������ܣ�ͬfree(ptr)���ͷ��ڴ��
���룺�ڴ���ַ(void *)
���أ���
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
�������ܣ����������ڴ�飬ʹ���б��Ϊfree���ڴ�飬�����ϡ��߼��Ϻ���һ��
���룺��
���أ���
*/
void mDefrag (void)
{
    
}


#ifdef _M_SHELL_
/*
�������ܣ�չʾ���е��ڴ�飬�������ֻ��shell��ʹ��
���룺��
���أ���
*/
void mListBlock4Shell (void)
{
}

/*
�������ܣ�ʹ���������ͷ��ڴ�飬�������ֻ��shell��ʹ��
���룺�ڴ������(char *)
���أ���
*/
void mFree4Shell (char * pchName)
{
}

#endif
