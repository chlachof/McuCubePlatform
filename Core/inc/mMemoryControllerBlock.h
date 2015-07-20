#ifndef _MCB_
#define _MCB_

#ifndef MCB_SIZE
#define MCB_SIZE            (10240)
#endif

#define BLOCK_NAME_LEN_MAX  (50)

struct MemoryBlockList
{
    char m_pchName[BLOCK_NAME_LEN_MAX];
    void * m_pvAddress;
    unsigned long m_ulSize;
    struct MemoryBlockList * m_pstNest;
}

#endif