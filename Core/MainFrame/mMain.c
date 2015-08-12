#include "stdlib.h"
#include "mMemoryControllerBlock.h"
#include "mShell.h"

void test1(unsigned char argc, char ** argv, struct CmdList cmd)
{
}

struct CmdList g_stCmdList[] = 
{
    {1, test1, "t1", 0, 2, 0, "", "", NULL},
    {2, test1, "t2", 0, 3, 0, "", "", NULL},
    {2, test1, "t3", 4, 4, 0, "", "", NULL},
};

int main()
{
    mRegisterCmdList(sizeof(g_stCmdList)/sizeof(struct CmdList), g_stCmdList);
    mRunCmd();
    return 0;
}