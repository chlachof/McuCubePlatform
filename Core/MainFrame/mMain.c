#include "mMemoryControllerBlock.h"

int main()
{
    char * test1 = (char *)mMalloc("test1", 10);
    char * test2 = (char *)mMalloc("test2", 20);
    char * test3 = (char *)mMalloc("test3", 30);
    char * test4 = (char *)mMalloc("test4", 10);
    char * test5 = (char *)mMalloc("test5", 20);
    char * test6 = (char *)mMalloc("test6", 30);
    char * test7 = (char *)mMalloc("test7", 15);
    char * test8 = (char *)mMalloc("test8", 25);
    char * test9 = (char *)mMalloc("test9", 35);
    char * test10 = (char *)mMalloc("test10", 70);
    mFree(test2);
    mFree(test4);
    mFree(test5);
    mFree(test3);
    mFree(test8);
    char * test11 = (char *)mMalloc("test11", 60);
    mDefrag();
    char * test12 = (char *)mMalloc("test12", 60);
    return 0;
}
