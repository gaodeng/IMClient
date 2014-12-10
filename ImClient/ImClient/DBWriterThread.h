#pragma once
#include "Thread.h"
#include "Sqlite.h"
#include <queue>
typedef struct  MSGBUF_Tag
{
	msginfo recvmsg;
	msginfo sendmsg;
}MsgBuf, * PMSGBUF;

#define  BUF_WRITING 1  //д
#define  BUF_READING 2   //��
#define  BUF_IDLE 3   //û��дҲû�ж�
#define  BUF_READYTOREAD  4 // ������д�� ׼����

/*
	������Ϣ�洢�߳���
	����Ϣָ����ڶ����У� ����ѭ���洢��sqlite��
*/

class CDBWriterThread:public CThread
{
public:
	CDBWriterThread(void);
	~CDBWriterThread(void);
	volatile int     m_recvBufStatus;  //��������״̬ 1��д 2������ 3��û��дҲû�ж�
	volatile int	 m_sendBufStatus; // ͬ��
	
	//���������洢�������Ϣ
	std::queue<msginfo*>  m_normalmsgQueue;
	BOOL             m_bQueueWritinng;
private:
	BOOL Run();
};


