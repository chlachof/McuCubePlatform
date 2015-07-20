#include "mMemoryControllerBlock.h"

struct MemoryBlockList  g_stMemoryInUse = NULL;
struct MemoryBlockList  g_stMemoryFree = NULL;

/*
as same as malloc(size), and add a parameter called "name" for showing
*/
void * mMalloc (char * pchName, unsigned long m_ulSize)
{
}

/*
as same as free(ptr)
*/
void mFree (void * pvAddress)
{
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
void mBlockList ()
{
}

/*
as same as mFree, but use name to free blocks, however, it is usually called by shell
*/
void mFreeByName (char * pchName)
{
}
