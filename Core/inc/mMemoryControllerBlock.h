#ifndef _M_MCB_
#define _M_MCB_

#ifndef MCB_SIZE
#define MCB_SIZE            (100)
#endif

#define MCB_ALIGNMENT       (4)
#define MCB_ALIGNMENT_MASK  (MCB_ALIGNMENT - 1)

struct MemoryBlockList
{
    unsigned long m_ulSize;             //�ڴ���С
    const char * m_pchName;             //�ڴ������
    struct MemoryBlockList * m_pstNext; //nextָ��
};

union MemoryHeap
{
    volatile unsigned long  ulDummy;
    unsigned char           pucMCB[MCB_SIZE]
};

/*
�������ܣ�ͬmalloc(size)������name������������shell��չʾ��ʬ��Ƭ����Ȼ��ʹ�����ִ���nullҲ���ԣ�Ĭ������Ϊ"N/A"
���룺�ڴ������(char *)���ڴ���С(unsigned long)
���أ��ڴ���ַ(void *)
*/
void * mMalloc (char * pchName, unsigned long ulSize);

/*
�������ܣ�ͬfree(ptr)���ͷ��ڴ��
���룺�ڴ���ַ(void *)
���أ���
*/
void mFree (void * pvAddress);

/*
�������ܣ����������ڴ�飬ʹ���б��Ϊfree���ڴ�飬�����Ϻ���һ��
���룺��
���أ���
*/
void mDefrag (void);

#ifdef _M_SHELL_
/*
�������ܣ�չʾ���е��ڴ�飬�������ֻ��shell��ʹ��
���룺��
���أ���
*/
void mListBlock4Shell (void);

/*
�������ܣ�ʹ���������ͷ��ڴ�飬�������ֻ��shell��ʹ��
���룺�ڴ������(char *)
���أ���
*/
void mFree4Shell (char * pchName);
#endif

#endif
