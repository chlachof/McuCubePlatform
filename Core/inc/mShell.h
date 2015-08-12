#ifndef _M_SHELL_
#define _M_SHELL_

#define SHELL_ECHO

#ifndef SHELL_MAX_LINE_LENGTH
#define SHELL_MAX_LINE_LENGTH   (64)    //������󳤶�
#endif

#ifndef SHELL_MAX_LINE
#define SHELL_MAX_LINE          (10)    //�������
#endif

#ifndef SHELL_MAX_ARGC
#define SHELL_MAX_ARGC          (10)    //����������
#endif

struct CmdList
{
    unsigned short      m_usId;                 //ID��
    void             (* m_pfFunction) (unsigned char, char **, struct CmdList); //����ָ��
    const char *        m_pchCmd;               //������
    unsigned char       m_ucArgMin;             //��С��������
    unsigned char       m_ucArgMax;             //����������
    unsigned char       m_ucArgType;            //��������
    const char *        m_pchDescription;       //��������
    const char *        m_pchSyntax;            //�����﷨
    struct CmdList *    m_pstNext;
};

struct CmdLineList
{
    char                    m_pchCmd[SHELL_MAX_LINE_LENGTH];
    struct CmdLineList *    m_pstNext;
    struct CmdLineList *    m_pstPrev;
};

/*
�������ܣ�ע��һ������
���룺
���أ��ɹ�(0),�ڴ治��(-12)
*/
int mRegisterCmd (void (* m_pfFunction) (unsigned char, char **, struct CmdList), const char * pchCmd, unsigned char ucArgMin, unsigned char m_ucArgMax, const char * pchDescription, const char * pchSyntax);

/*
�������ܣ�ע��һ�������б�
���룺
���أ��ɹ�(0)
*/
int mRegisterCmdList (unsigned char ucListSize, struct CmdList * pstCmdList);

/*
�������ܣ���ʼִ������
���룺��
���أ���
*/
void mRunCmd (void);

#endif










/* ���Դ���
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