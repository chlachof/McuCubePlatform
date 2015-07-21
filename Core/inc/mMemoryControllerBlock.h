#ifndef _MCB_
#define _MCB_

#ifndef MCB_SIZE
#define MCB_SIZE            (10240)
#endif

#define BLOCK_NAME_LEN_MAX  (10)

struct MemoryBlockList
{
    const char * m_pchName;
    unsigned long m_ulSize;
    struct MemoryBlockList * m_pstNext;
}

#endif