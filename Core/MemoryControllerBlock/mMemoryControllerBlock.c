#include "mMemoryControllerBlock.h"

const char      g_pchFree[5] = "free";
const char      g_pchNA[4] = "N/A";
unsigned char   g_pucMCB[MCB_SIZE] = {0};
struct MemoryBlockList * g_pstMemoryBlock = NULL;

/*
as same as malloc(size), and add a parameter called "name" for showing
*/
void * mMalloc (char * pchName, unsigned long ulSize)
{
    struct MemoryBlockList * pstMemoryTemp = NULL;
    struct MemoryBlockList * pstMemoryInsert = NULL;
    
    if(m_ulSize <= 0 || ulSize > MCB_SIZE) 
    {
        return NULL;
    }
    
    /*
    init g_pstMemoryBlock
    */
    if(g_pstMemoryBlock == NULL)
    {
        if(MCB_SIZE < sizeof(struct MemoryBlockList))
        {
            return NULL;
        }
        g_pstMemoryBlock = g_pucMCB;
        g_pstMemoryBlock->m_pchName = g_pchFree;
        g_pstMemoryBlock->m_ulSize = MCB_SIZE - sizeof(struct MemoryBlockList);
        g_pstMemoryBlock->m_pstNext = NULL;
    }
    
    /*
    select suitable block for the newcomer. the smallest but bigger than the newcomer
    */
    pstMemoryTemp = g_pstMemoryBlock;
    
    while(pstMemoryTemp != NULL)
    {
        if(pstMemoryTemp->m_pchName == g_pchFree)
        {
            if(pstMemoryTemp->m_ulSize > (ulSize + sizeof(struct MemoryBlockList)))
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
        pstMemoryTemp = pstMemoryInsert + pstMemoryInsert->m_ulSize - ulSize;
        pstMemoryTemp->m_ulSize = ulSize;
        pstMemoryTemp->m_pstNext = pstMemoryInsert->m_pstNext;
        
        pstMemoryInsert->ulSize -= (ulSize + sizeof(struct MemoryBlockList));
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
    
    return (pstMemoryTemp + sizeof(struct MemoryBlockList));
}

/*
as same as free(ptr)
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
            if(pvAddress == (pstMemoryTemp + sizeof(struct MemoryBlockList)))
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
to make all free blocks as one
*/
void mMemoryArrange (void)
{
}

/*
list all blocks
*/
void mBlockList (void)
{
}

/*
as same as mFree, but use name to free blocks, however, it is usually called by shell
*/
void mFreeByName (char * pchName)
{
}
