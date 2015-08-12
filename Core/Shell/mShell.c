#include "string.h"
#include "stdlib.h"
#include "mShell.h"
#include "mMemoryControllerBlock.h"


static char g_pchCmdLineCache[SHELL_MAX_LINE_LENGTH];
static struct CmdList g_stCmdNodeCache;
static struct CmdList * g_pstCmdList = NULL;
static struct CmdLineList * g_pstCmdLineList = NULL;


char * mGetLine(char * pchLine);
unsigned char mMakeArgumentValue(char * pchLine, char ** ppchArgumentValue);


/*
函数功能：注册一张命令列表
输入：
返回：成功(0)
*/
int mRegisterCmdList (unsigned char ucListSize, struct CmdList * pstCmdList)
{
    struct CmdList * pstCmdNode = NULL;
    struct CmdList * pstCmdNodePrev = NULL;
    unsigned char i;
    
    pstCmdNode = pstCmdList;
    i = ucListSize - 1;
    while(i--)
    {
        pstCmdNode->m_pstNext = pstCmdNode + 1;
        pstCmdNode ++;
    }
    
    if(g_pstCmdList == NULL)
    {
        g_pstCmdList = pstCmdList;
    }
    else 
    {
        pstCmdNodePrev = g_pstCmdList;
        while(pstCmdNodePrev->m_pstNext != NULL)
        {
            pstCmdNodePrev = pstCmdNodePrev->m_pstNext;
        }
        pstCmdNodePrev->m_pstNext = pstCmdNode;
    }
    
    return (0);
}

/*
函数功能：获取命令字符串
输入：字符串地址(char *)
返回：字符串地址(char *)
*/
char * mGetLine(char * pchLine)
{
    return (pchLine);
}

/*
函数功能：分割命令字符串
输入：
返回：参数个数(unsigned char)
*/
unsigned char mMakeArgumentValue(char * pchLine, char * ppchArgumentValue[])
{
    unsigned char ucArgumentCount = 0;
    struct CmdLineList * pstCmdLineNode = NULL;
    struct CmdLineList * pstCmdLineNodeNext = NULL;
    unsigned char i;
    unsigned char ucTextFlag = 0;
    
    if(pchLine == NULL)
    {
        return (0);
    }
    
    if(strlen(pchLine) > (SHELL_MAX_LINE_LENGTH-1))
    {
        return (0);
    }
    
    if(g_pstCmdLineList == NULL)
    {
        g_pstCmdLineList = (struct CmdLineList *)mMalloc("CmdLineList", sizeof(struct CmdLineList) * SHELL_MAX_LINE);
        if(g_pstCmdLineList == NULL)
        {
            return (0);
        }
        memset(g_pstCmdLineList, 0, sizeof(struct CmdLineList) * SHELL_MAX_LINE);
        
        pstCmdLineNode = g_pstCmdLineList;
        i = SHELL_MAX_LINE - 1;
        while(i--)
        {
            pstCmdLineNodeNext = pstCmdLineNode + 1;
            pstCmdLineNode->m_pstNext = pstCmdLineNodeNext;
            pstCmdLineNodeNext->m_pstPrev = pstCmdLineNode;
            pstCmdLineNode ++;
        }
        pstCmdLineNode->m_pstNext = g_pstCmdLineList;
        g_pstCmdLineList->m_pstPrev = pstCmdLineNode;
    }
    
    strcpy(g_pstCmdLineList->m_pchCmd, pchLine);
    g_pstCmdLineList = g_pstCmdLineList->m_pstNext;
    
    i = 0;
    while((pchLine[i] != '\0') && (pchLine[i] != '\r'))
    {
        if((pchLine[i] == ' ') || (pchLine[i] == ','))
        {
            if(ucTextFlag == 1)
            {
                ucTextFlag = 0;
                pchLine[i] = '\0';
            }
        }
        else 
        {
            if(ucTextFlag == 0)
            {
                ucTextFlag = 1;
                if(ucArgumentCount < SHELL_MAX_ARGC)
                {
                    if(ppchArgumentValue != NULL)
                    {
                        ppchArgumentValue[ucArgumentCount] = &pchLine[i];
                        ucArgumentCount ++;
                    }
                }
                else 
                {
                    break;
                }
            }
        }
        i++;
    }
    pchLine[i] = '\0';
    if(ppchArgumentValue != NULL)
    {
        ppchArgumentValue[ucArgumentCount] = NULL;
    }
    
    return (ucArgumentCount);
}

/*
函数功能：开始执行命令
输入：无
返回：无
*/
void mRunCmd (void)
{
    unsigned char ucArgumentCount = 0;
    char * ppchArgumentValue[SHELL_MAX_ARGC+1];
    struct CmdList * pstCmdNode = NULL;
    
    mGetLine(g_pchCmdLineCache);
    ucArgumentCount = mMakeArgumentValue(g_pchCmdLineCache, ppchArgumentValue);
    
    if(ucArgumentCount)
    {
        pstCmdNode = g_pstCmdList;
        while(pstCmdNode != NULL)
        {
            if(strcasecmp(pstCmdNode->m_pchCmd, ppchArgumentValue[0]) == 0)
            {
                if( ((ucArgumentCount-1) >= pstCmdNode->m_ucArgMin) && ((ucArgumentCount-1) <= pstCmdNode->m_ucArgMax))
                {
                    memcpy((char *)(&g_stCmdNodeCache), (char *)(pstCmdNode), sizeof(struct CmdList));
                    pstCmdNode->m_pfFunction(ucArgumentCount, ppchArgumentValue, g_stCmdNodeCache);
                }
                break;
            }
            pstCmdNode = pstCmdNode->m_pstNext;
        }
    }
}
