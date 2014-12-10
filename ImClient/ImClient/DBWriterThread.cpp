#include "StdAfx.h"
#include "DBWriterThread.h"


//�ٽ��� ��֤ ������Ϣʱ �洢��Ϣ�Ĺ���
CRITICAL_SECTION g_sendcs ;

//���sqlite�е�һЩ�ַ�����Ҫ����ת���ַ�
void HandleString(string & str)
{
	char* str1 = "'";
	char* str2 = "''";
	string::size_type  pos = 0;
	while((pos = str.find(str1, pos)) != string::npos)
	{
		str.replace(pos, strlen(str1), str2 );
		pos += strlen(str2);
	}
}


CDBWriterThread::CDBWriterThread(void)
{
	m_recvBufStatus = BUF_IDLE;
	m_sendBufStatus = BUF_IDLE;
	m_bQueueWritinng = FALSE;
	InitializeCriticalSection(&g_sendcs);
}


CDBWriterThread::~CDBWriterThread(void)
{
	DeleteCriticalSection(&g_sendcs);
	if (!m_normalmsgQueue.empty())
	{
		for (int i=0; i < m_normalmsgQueue.size(); i++)
		{
			msginfo* p =m_normalmsgQueue.front();
			SAFE_DELETE(p);
			m_normalmsgQueue.pop();
		}
	}
}

BOOL CDBWriterThread::Run()
{
// 	if (m_recvBufStatus != BUF_READYTOREAD)
// 	{
// 		return TRUE;
// 	}
#if 0
	CSqlite*   p = CSqlite::GetIntstance();
	if (m_recvBufStatus == BUF_READYTOREAD)
	{
		m_recvBufStatus  = BUF_READING;   //��ʱ����д״̬
		PMSGBUF pMsgBuf = (PMSGBUF)m_pPara;
		p->GetMsgDbInterface()->AddMsg(pMsgBuf->recvmsg);
		//д��
		m_recvBufStatus = BUF_IDLE; //����������־��Ϊ����
		Sleep(1);
	}
	if (m_sendBufStatus == BUF_READYTOREAD)
	{
		EnterCriticalSection(&g_sendcs);
		m_sendBufStatus =  BUF_READING;
		PMSGBUF pMsgBuf = (PMSGBUF)m_pPara;
		p->GetMsgDbInterface()->AddMsg(pMsgBuf->sendmsg);
		m_sendBufStatus = BUF_IDLE;
		LeaveCriticalSection(&g_sendcs);
	}

	Sleep(1);
	return TRUE;
#endif 
	msginfo* pMsgInfo = NULL;
	EnterCriticalSection(&g_sendcs);
	if (m_normalmsgQueue.size() != 0)
	{
		pMsgInfo = m_normalmsgQueue.front();
		HandleString(pMsgInfo->msgcontent);
		m_normalmsgQueue.pop();
		LeaveCriticalSection(&g_sendcs);

		CSqlite::GetIntstance()->GetMsgDbInterface()->AddMsg(*pMsgInfo);
		SAFE_DELETE(pMsgInfo);
		Sleep(20);
	}
	else
	{
		LeaveCriticalSection(&g_sendcs);
		Sleep(100);//Sleep(50);
	}
	return TRUE;
}
