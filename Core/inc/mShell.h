#ifndef _M_SHELL_
#define _M_SHELL_

#define SHELL_ECHO

#ifndef SHELL_MAX_LINE_LENGTH
#define SHELL_MAX_LINE_LENGTH   (64)    //单行最大长度
#endif

#ifndef SHELL_MAX_LINE
#define SHELL_MAX_LINE          (10)    //最大行数
#endif

#ifndef SHELL_MAX_ARGC
#define SHELL_MAX_ARGC          (10)    //最大参数个数
#endif

struct CmdList
{
    unsigned short      m_usId;                 //ID号
    void             (* m_pfFunction) (unsigned char, char **, struct CmdList); //函数指针
    const char *        m_pchCmd;               //命令名
    unsigned char       m_ucArgMin;             //最小参数个数
    unsigned char       m_ucArgMax;             //最大参数个数
    unsigned char       m_ucArgType;            //参数类型
    const char *        m_pchDescription;       //命令描述
    const char *        m_pchSyntax;            //命令语法
    struct CmdList *    m_pstNext;
};

struct CmdLineList
{
    char                    m_pchCmd[SHELL_MAX_LINE_LENGTH];
    struct CmdLineList *    m_pstNext;
    struct CmdLineList *    m_pstPrev;
};

/*
函数功能：注册一个命令
输入：
返回：成功(0),内存不足(-12)
*/
int mRegisterCmd (void (* m_pfFunction) (unsigned char, char **, struct CmdList), const char * pchCmd, unsigned char ucArgMin, unsigned char m_ucArgMax, const char * pchDescription, const char * pchSyntax);

/*
函数功能：注册一张命令列表
输入：
返回：成功(0)
*/
int mRegisterCmdList (unsigned char ucListSize, struct CmdList * pstCmdList);

/*
函数功能：开始执行命令
输入：无
返回：无
*/
void mRunCmd (void);

#endif










/* 测试代码
#include "stdlib.h"
#include "mMemoryControllerBlock.h"
#include "mShell.h"

void test1(unsigned char argc, char ** argv)
{
}

struct CmdList g_stCmdList[] = 
{
    {test1, "t1", 0, 3, "t1", "t1 <p1> <p2> <p3>", NULL},
    {test1, "t2", 0, 2, "t2", "t2 <p1> <p2>",      NULL},
    {test1, "t3", 0, 1, "t3", "t3 <p1>" ,          NULL},
};

int main()
{
    mRegisterCmdList(sizeof(g_stCmdList)/sizeof(struct CmdList), g_stCmdList);
    mRunCmd();
    return 0;
}
*/