#include "stdlib.h"
#include "mMemoryControllerBlock.h"
#include "mScheduler.h"

int test1(void * pvArg)
{
    return 1;
}

int main()
{
    mRegisterThread(test1, 1, "1", NULL, 10);
    mRegisterThread(test1, 2, "2", NULL, 10);
    mRegisterThread(test1, 2, "3", NULL, 10);
    mRegisterThread(test1, 2, "4", NULL, 10);
    mRegisterThread(test1, 2, "5", NULL, 10);
    mRegisterThread(test1, 3, "6", NULL, 10);
    mRegisterThread(test1, 3, "7", NULL, 10);
    mRunScheduler();
    return 0;
}