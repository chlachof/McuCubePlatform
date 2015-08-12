/*
chlachof@126.com
2015.7.30
�ڴ����ģ�飬�������ͷţ���Ƭ����չʾ
*/
#include "stdlib.h"
#include "string.h"
#include "mMemoryControllerBlock.h"


static const char               g_pchFree[5] = "free";
static const char               g_pchNA[4] = "N/A";
static union MemoryHeap         g_unHeap;

static struct MemoryBlockList * g_pstMemoryBlock = (struct MemoryBlockList *)g_unHeap.m_pucMCB;

/*
�������ܣ�ͬmalloc(size)������name������������shell��չʾ��ʬ��Ƭ����Ȼ��ʹ�����ִ���nullҲ���ԣ�Ĭ������Ϊ"N/A"��ע�⣺��Ҫȡ��Ϊ"free"���������
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
    
    /*
    ���д���2���Ľṹ���С������ָ�������ڴ�飬����ֱ��ռ������ڴ��
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
�������ܣ�ͬfree(ptr)���ͷ��ڴ��
���룺�ڴ���ַ(void *)
���أ���
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
                ǰһ���ڴ��Ϊfree
                */
                if(pstMemoryTempNext->m_pchName == g_pchFree)
                {
                    pstMemoryTemp->m_pstNext = pstMemoryTempNext->m_pstNext;
                    pstMemoryTemp->m_ulSize += pstMemoryTempNext->m_ulSize + sizeof(struct MemoryBlockList);
                }
                
                /*
                ��һ���ڴ��Ϊfree
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
�������ܣ����������ڴ�飬ʹ���б��Ϊfree���ڴ�飬�����ϡ��߼��Ϻ���һ������û��MMU֧�֣���ʵ��Ƭ������û��ʵ�ֵģ���Ϊ�����ַ�����˱仯��������ȥ��ָ��û���ı䣬�ʲ����Žӿ�
���룺��
���أ���
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
        �����free�飬���ó��Լ��������ַ
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
        �����м���������������ַ
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
