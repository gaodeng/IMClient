#include "StdAfx.h"
#include "IMSocket.h"

//#include "MsgData.h"
#include "MsgOfDelay.h"
#include "ProfileRequest.h"
#include "ProfileResponse.h"
#include "MsgOfArrival.h"
#include "LoginResponse.h"
#include "ConnectResponse.h"
#include "FriendGroup.h"
#include "SourceDownloadThread.h"
extern const char* const_type;
extern const char* const_msgtype ;
extern const char* const_type_normal_msgval ;
extern const char* const_type_login_responseval;
extern const char* const_type_logout_responseval;
extern const char* const_token;
extern const char* const_type_connect_requestval;
extern const char* const_msgtype_connect_requestval;
extern const char* const_type_connect_responseval;
extern const char* const_allocatedMsgServ ;
extern const char* const_host ;
extern const char* const_port ;
extern const char* const_msgcontent ;
extern const char* cosnt_errorcode ;
extern const char* const_errorMsg ;
extern const char* const_fromid;
extern const char* const_toid ;
extern const char* const_memberid ;
extern CRITICAL_SECTION g_sendcs ;

// �ͻ��˾������ö��߳�

//ȫ�ֱ�����Ϣ������ 
MsgBuf  msgBuf;
MsgBuf  msgBugSend;

//�����ݴ浽sqlite֮ǰ ����Ҫ����
// void HandleString(std::string & str)
// {
// 	char* str1 =  "'";
// 	char* str2 = "''";
// 	std::string::size_type  pos = 0;
// 	while((pos = str.find(str1, pos)) != std::string::npos)
// 	{
// 		str.replace(pos, strlen(str1), str2 );
// 		pos += strlen(str2);
// 	}
// }

//����Ϣjson�������ṹ��
 //��Ҫ�ڽ����߳��м���
void  CIMSocket::MakeMsgBuf(const Json::Value & val, E_FROMORTOTYPE fromorto)
{
#if 0 
	if (val[const_type].asString() != const_type_normal_msgval)
	{
		OutputDebugString(_T("Not MsgJson \n"));
		return;
	}

	msginfo*  pMsgBug = NULL;
	//pMsgBug = fromorto==E_TOFRIEND? &msgBugSend:&msgBufRecv;
	if (fromorto == E_FROMFRIEND) 
	{
		//�յ� ��Ϣ
		//���ڽ��յ���Ϣ�� m_recvBufStatus ����ΪΪBUF_IDLE 
		//ԭ�����ڣ� �����յ�ʱ�򻺳�����æ����һֱ����0 ���ֽ�
		//Ҳ���ǲ�������sockect��������ȡ�ֽ�
		if (m_pThread->m_recvBufStatus != BUF_IDLE)
		{
			return ;
		}
		pMsgBug = &(msgBuf.recvmsg);
	}
	else  //E_TOFRIEND
	{
		//���͵���Ϣ����Ϣ��������������
		//�ٽ����� ��֤��ԭ����
		pMsgBug = &(msgBuf.sendmsg);
		while(m_pThread->m_sendBufStatus != BUF_IDLE)
		{
			//��ʱ��Ƭ���������߳�
			//������BUF_IDLE ��ʾ����ȥ��û����
			Sleep(0);
		}
		EnterCriticalSection(&g_sendcs); 
	
	}
	
	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	


	if (fromorto == E_FROMFRIEND)
		m_pThread->m_recvBufStatus = BUF_WRITING;
	else
		m_pThread->m_sendBufStatus = BUF_WRITING;
	
	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //�����û�����ȥ����Ϣ
	{
		pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
		pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //��ϢID�����ݿ���������

	//��ʱ����������־��Ϊ�ɶ���

	if (fromorto == E_FROMFRIEND)
		m_pThread->m_recvBufStatus = BUF_READYTOREAD;
	else
		m_pThread->m_sendBufStatus = BUF_READYTOREAD;
	if (fromorto == E_TOFRIEND)
	{
		LeaveCriticalSection(&g_sendcs);
	}
	
	Sleep(0);
#endif
	//��������ʱ����Ϣ�洢�ڶ�����
	if (val[const_type].asString() != const_type_normal_msgval)
	{
		OutputDebugString(_T("Not MsgJson \n"));
		return;
	}

	msginfo*  pMsgBug =  new msginfo;
	//pMsgBug = fromorto==E_TOFRIEND? &msgBugSend:&msgBufRecv;
	if (fromorto == E_FROMFRIEND) 
	{
		//�յ����ѻ�����ϵ�˵���Ϣ
	}
	else  //E_TOFRIEND
	{
		//���͵����ѻ�����ϵ�˵���Ϣ
	}

	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //��ǰ�û�����ȥ����Ϣ
	{
		pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
		pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //��ϢID�����ݿ���������
	pMsgBug->font = GetMsgfont();
	EnterCriticalSection(&g_sendcs); 
	m_pThread->m_normalmsgQueue.push(pMsgBug);
	LeaveCriticalSection(&g_sendcs);

	//��ʱ����������־��Ϊ�ɶ���
}

//////////////////////////////////////////////////////////////////////////////////////////////

CIMSocket::CIMSocket(void)
{
	memset(m_sendBuf, 0x0, sizeof(m_sendBuf));
	memset(m_recvBuf, 0x0, sizeof(m_recvBuf));
	memset(m_tempBuf, 0x0, sizeof(m_tempBuf));
	m_bConnecting = FALSE;
	m_nLength  = 0;
	m_nTempLen = 0;
	m_bFinished = TRUE;
	m_hWnd = NULL;
	m_pszJson = NULL;
	m_nPos = 0;
	m_serverType =E_NOSERVERCONNECT;
	//m_pThread = NULL;
	m_nReply = 0;  //�������� ��ʼ��Ϊ0 
	m_pHeartBeatThread = NULL;
	m_pThread = NULL;
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszServ[24] = {0x0};
	pIni->ReadString(_T("loginserver"), _T("value"), NULL, tszServ, 23);
	char* szServ = NULL;
	T2C(&szServ, tszServ);
	strcpy_s(m_szLoginServerIp, szServ);
	SAFE_ARRYDELETE(szServ);
	m_nLoginPort = pIni->ReadInt(_T("loginport"), _T("value"), NULL);
	m_nRecvLen = 0;
	m_bContinue = FALSE;
	SetMsgfont(CFontInfo());
}


CIMSocket::~CIMSocket(void)
{
	m_pThread->StopThread();
	m_pHeartBeatThread->StopThread();
	//m_pFriendUpdateThread->StopThread();
	SAFE_ARRYDELETE(m_pszJson);
	SAFE_DELETE(m_pThread);
	SAFE_DELETE(m_pHeartBeatThread);
}

void CIMSocket::Create(HWND hwnd)
{
	m_hWnd = hwnd;
	InitDBWirterThread();
}

BOOL CIMSocket::InitSocket(E_SERVERTPE type, int port/* = SERVERPORT*/, LPCSTR lpcStr/* = SERVERIP*/)
{
	m_serverType = type;
	WSADATA wsData;
	if (WSAStartup(MAKEWORD(2,2), &wsData) != 0)
	{
		return FALSE;
	}
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(INVALID_SOCKET == m_sock)
	{
		return FALSE;
	}

	sockaddr_in  serverAddr;
	serverAddr.sin_family = AF_INET;
	if (type== E_LOGINSERVER)
	{
		serverAddr.sin_port = htons(m_nLoginPort);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(m_szLoginServerIp);
	}
	else
	{
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.S_un.S_addr = inet_addr(lpcStr);
	}
	
	if (WSAAsyncSelect(m_sock,m_hWnd, WM_SOCK_CONNECT, FD_CONNECT) == SOCKET_ERROR)
	{
		OutputDebugString(_T("SOCKET ERROR \n"));
		return FALSE;
	}

	//��ʱ�������첽�ģ� ��Ҫ��WM_SOCK_CONNECT���ж��Ƿ��Ѿ�����
	connect(m_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	return TRUE;
}

BOOL CIMSocket::InitLoginServer()
{
	if (m_bConnecting == TRUE)
	{
		return FALSE;
	}
	if (!InitSocket(E_LOGINSERVER))
	{
		ExitSocket();
		return FALSE;
	}
	return TRUE;
}

BOOL  CIMSocket::InitMsgSock(int port, const char* strip)
{
	return 0;
}

//�ر��׽��֣� �����Դ
void CIMSocket::ExitSocket(void)
{
#if _DEBUG
	if (m_serverType == E_LOGINSERVER)
	{
		OutputDebugString(_T("LOGIN��SERVER DISCONNECTED ,CLOSE  SOCKET\n"));
	}
	else
	{
		OutputDebugString(_T("MSG��SERVER DISCONNECTED , CLOSE  SOCKET\n"));
	}
#endif
	m_serverType = E_NOSERVERCONNECT;
	closesocket(m_sock);
	m_bConnecting = FALSE;
	WSACleanup();
}

LRESULT CIMSocket::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//�������
	bHandled = TRUE;
	int nErrorCode = WSAGETSELECTERROR(lParam);
	if (WSAGETSELECTERROR(lParam))
	{
		//WSAEHOSTDOWN
		if (nErrorCode ==WSAEHOSTDOWN)
		{
			OutputDebugString(_T(" SOCKET CLOSED \n"));
			//����Ͽ�
			//m_bConnecting = FALSE;
			//closesocket()
		}
		ExitSocket();
		return 0;
	}
	int wPEvent = WSAGETSELECTEVENT(lParam);
	switch(uMsg)
	{
	case WM_SOCK_CONNECT:
		{
// 			if (FD_WRITE ==wPEvent)
// 			{
// 				//SendData();
// 			}
			if(FD_CONNECT == wPEvent)
			{
				//�ɹ����ӣ� 
				
#if 1
				if (m_serverType == E_LOGINSERVER)
				{
					OutputDebugString(_T("LOGIN SERVER CONNECT SUCCEDDED\n"));
					m_bConnecting = TRUE;
					SendData(m_strLoginJson);
				}
				else
				{
					OutputDebugString(_T("MSG SERVER CONNECT SUCCEDDED\n"));
					//����ȷ����������Ϣ������
					//���б�Ҫ��ʼ�����ݿ�д�߳�
					InitDBWirterThread() ; 
					InitHeartBeatThread();
					//�����ǲ���
// 					ConnectRequest request;
// 					//request.Init(const_type_connect_requestval, const_msgtype_connect_requestval);
// 					request.m_strLoginId = m_strLoginId;//g_user.m_userInfo.id/*"13622882929"*/;
// 					request.m_strToken = m_strToken;
					
					Json::Value val;
					m_connnectRequest.MakeJson(val,const_type_connect_requestval,const_msgtype_connect_requestval);
					SendData(val);
				}
				
#endif
				//����������һ��WSAAsyncSelect��ǰ��������
				if (WSAAsyncSelect(m_sock, m_hWnd, WM_SOCK_RWORCLOSE,FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
				{
					ExitSocket();
					OutputDebugString(_T("WSAAsyncSelect ERROR \n"));
				}
			}
			break;
		}
	case WM_SOCK_RWORCLOSE:
		{
			if (FD_WRITE == wPEvent)
			{
				//׼���÷���
			}
			else if (FD_READ == wPEvent)
			{
				//׼���ý�������
				ReceiveData();
			}

			else if (FD_CLOSE == wPEvent)
			{
				//�ر�
				Close();
			}
			break;
		}
	default: 
			bHandled = FALSE;
			break;
	
	}
	return 0;
}

void CIMSocket::InitDBWirterThread()
{
	//��ʼ���߳���
	m_pThread = new CDBWriterThread();
	//���ò���, ���뻺�����ṹ��ָ��
	m_pThread->SetPara((UINT_PTR)(&msgBuf));
	m_pThread->StartThread();
}

void CIMSocket::InitHeartBeatThread()
{
	m_pHeartBeatThread = new CHeartBeatThread;
	//m_pHeartBeatThread->StartThread();
}

#ifdef _DEBUG

void PrintSendOrRecvStr(const char* buf , int len, const TCHAR* type)
{

	char* pOut = new char[len+1];
	memset(pOut, 0x0, len+1);
	memcpy(pOut, buf, len);
	TCHAR* ptOut = NULL;
	
	C2T(&ptOut, pOut);
	OutputDebugString(type);
	OutputDebugString(ptOut);
	SAFE_ARRYDELETE(pOut);
	SAFE_ARRYDELETE(ptOut);
}
#endif

void CIMSocket::HandleStickPackeage(BOOL bWhenNewRecv /* = TRUE */)
{
	if (m_nTempLen > 0)
	{
		// ���ֶϰ���Ҫ����
		int nLen1 = 0;// �Ѿ�������ֽڳ���

		int nRemain = m_nTempLen; // �ϰ�ʣ�೤��
		while (nLen1 < m_nTempLen)
		{
			int nLen2 = 0;   // ÿ��Json������

			if (m_nTempLen-nLen1 < 4)
			{
				//ʣ��С��4���ֽ��޷����㳤��
				char* pTempBuf = new char[m_nTempLen - nLen1];
				memcpy(pTempBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
				memset(m_tempBuf, 0x0, MAX_NUM_BUF);
				m_nTempLen = m_nTempLen - nLen1;
				memcpy(m_tempBuf, pTempBuf, m_nTempLen);
				SAFE_ARRYDELETE(pTempBuf);
				break;
			}
			else
			{
				ByteToInt(&nLen2, m_tempBuf + nLen1);

				//JSon�� ����4���ֽڵĳ���
				if (nLen1 + nLen2+4 > m_nTempLen)
				{
					//�ϰ�
					char* pTempBuf = new char[m_nTempLen - nLen1];
					memcpy(pTempBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); //
					memset(m_tempBuf, 0x0, MAX_NUM_BUF);
					m_nTempLen = m_nTempLen - nLen1;
					memcpy(m_tempBuf, pTempBuf,m_nTempLen );
					SAFE_ARRYDELETE(pTempBuf);
					break;
				}
				else
				{
					m_pszJson = new char[nLen2 + 1];
					memset(m_pszJson, 0, nLen2 + 1);
					memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
					Json::Value val;
					if (!m_pszJson)
					{

					}
					Pack(val, m_pszJson);
					HandleJson(val);
	
					//m_nTempLen = 0;
				}
				nLen1  += nLen2 +4;
			}
		}
		if(nLen1 == m_nTempLen)
			m_nTempLen = 0;
	}
}


/*
	
*/
void CIMSocket::ReceiveData()
{
	memset(m_recvBuf, 0, MAX_NUM_BUF);
	//int nRecvLen = 0;
	int retVal = -1;
	//����Ҫѭ�����գ�δ�������ϵͳ���Զ�֪ͨ
	if (m_bConnecting)
	{
		if (m_nTempLen > 0)
		{
			// ���ֶϰ���Ҫ����
			int nLen1 = 0;// �Ѿ�������ֽڳ���
			
			int nRemain = m_nTempLen; // �ϰ�ʣ�೤��
			while (nLen1 < m_nTempLen)
			{
				int nLen2 = 0;   // ÿ��Json������

				if (m_nTempLen-nLen1 < 4)
				{
					//ʣ��С��4���ֽ��޷����㳤��
					memcpy(m_recvBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
					m_nPos = m_nTempLen - nLen1;
					break;
				}
				else
				{
					ByteToInt(&nLen2, m_tempBuf + nLen1);

					//JSon�� ����4���ֽڵĳ���
					if (nLen1 + nLen2+4 > m_nTempLen)
					{
						//�ϰ�
						memcpy(m_recvBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); // д������buffer
						m_nPos = m_nTempLen-nLen1 ; // ��־λ��
					}
					else
					{
						m_pszJson = new char[nLen2 + 1];
						memset(m_pszJson, 0, nLen2 + 1);
						memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
						Json::Value val;
						if (!m_pszJson)
						{

						}
						Pack(val, m_pszJson);
						HandleJson(val);
					}
					nLen1  += nLen2 +4;
				}
			}

			m_nTempLen = 0;	
		}
		else
			m_nPos = 0;

		retVal = recv(m_sock, m_recvBuf + m_nPos, MAX_NUM_BUF - m_nPos, 0);

		if (SOCKET_ERROR == retVal)
		{
			int nErrCode = WSAGetLastError();
			return;
		}
		else if (0 == retVal)
		{
	
			//�������ر�����
			return;
		}
		else if (retVal > 0 )
		{
			//���յ������ݴ�С
		}
	}
	////////////////////////

	if (retVal >0)
	{
		if (m_bContinue)
		{ // һ��Json  û�����꣬ ��������
			if (m_nLength > m_nRecvLen + retVal  + m_nPos)
			{
				memcpy(m_pszJson + m_nRecvLen, m_recvBuf, retVal);
				m_nRecvLen += retVal;
			}
			else
			{
				memcpy(m_pszJson + m_nRecvLen,m_recvBuf, m_nLength - m_nRecvLen);
				Json::Value val;
				Pack(val, m_pszJson);
				HandleJson(val);
				m_nTempLen = m_nRecvLen + retVal - m_nLength;
				memcpy(m_tempBuf, m_recvBuf + m_nLength - m_nRecvLen, m_nTempLen);
				
				//TODO:  bug here 
				//int x ;
				//ByteToInt(&x, m_tempBuf);
				///////////// Begin 
				HandleStickPackeage();

				/////////////////end
				//
				m_bContinue = FALSE;
				m_nRecvLen = 0;
			}
		}
		else
		{
			//��ȡ��ǰ���յİ��ĳ��ȣ�ǰ�ĸ��ֽ�Ϊ����
			if (retVal + m_nPos < 4)
			{
				//m_nPos += retVal;
				m_nTempLen = m_nPos + retVal;
				memcpy(m_tempBuf, m_recvBuf, m_nTempLen);
			}
			else
			{
				ByteToInt(&m_nLength, m_recvBuf);
				//Ϊ��Ҫ���յ�json�������ڴ�ռ�
				if (!m_pszJson)
				{
					m_pszJson = new char[m_nLength + 1];
					memset(m_pszJson, 0, m_nLength + 1);
				}

				//С��MAX_BUF�����
				if (m_nLength <= retVal + m_nPos  -4)
				{
					memcpy(m_pszJson, m_recvBuf + 4,m_nLength);
					Json::Value val;
					Pack(val, m_pszJson);
					HandleJson(val);
					m_nTempLen = retVal + m_nPos -m_nLength -4;
					memcpy(m_tempBuf, m_recvBuf + m_nLength + 4, m_nTempLen);
				}
				else 
				{	//��δ������
					memcpy(m_pszJson + m_nRecvLen,  m_recvBuf + 4,  retVal + m_nPos -4 );
					m_nRecvLen  +=  retVal + m_nPos - 4;
					m_bContinue = TRUE;
				}
			}
		}
	
		
	}
}

#if 0

void CIMSocket::ReceiveData()
{
	memset(m_recvBuf, 0, MAX_NUM_BUF);
	//int nRecvLen = 0;
	int retVal = -1;
	//����Ҫѭ�����գ�δ�������ϵͳ���Զ�֪ͨ
	if (m_bConnecting)
	{
		if (m_nTempLen > 0)
		{
			// ���ֶϰ���Ҫ����
			int nLen1 = 0;// �Ѿ�������ֽڳ���

			int nRemain = m_nTempLen; // �ϰ�ʣ�೤��
			while (nLen1 < m_nTempLen)
			{
				int nLen2 = 0;   // ÿ��Json������

				if (m_nTempLen-nLen1 < 4)
				{
					//ʣ��С��4���ֽ��޷����㳤��
					memcpy(m_recvBuf, m_tempBuf + nLen1, m_nTempLen - nLen1);
					m_nPos = m_nTempLen - nLen1;
					break;
				}
				else
				{
					ByteToInt(&nLen2, m_tempBuf + nLen1);

					//JSon�� ����4���ֽڵĳ���
					if (nLen1 + nLen2+4 > m_nTempLen)
					{
						//�ϰ�
						memcpy(m_recvBuf, m_tempBuf+nLen1, m_nTempLen-nLen1); // д������buffer
						m_nPos = m_nTempLen-nLen1 ; // ��־λ��
					}
					else
					{
						m_pszJson = new char[nLen2 + 1];
						memset(m_pszJson, 0, m_nLength + 1);
						memcpy(m_pszJson, m_tempBuf + nLen1 + 4, nLen2);
						Json::Value val;
						Pack(val, m_pszJson);
						HandleJson(val);
					}
					nLen1  += nLen2 +4;
				}
			}

			m_nTempLen = 0;	
		}
		else
			m_nPos = 0;

		retVal = recv(m_sock, m_recvBuf + m_nPos, MAX_NUM_BUF - m_nPos, 0);

		if (SOCKET_ERROR == retVal)
		{
			int nErrCode = WSAGetLastError();
			return;
		}
		else if (0 == retVal)
		{

			//�������ر�����
			return;
		}
		else if (retVal > 0 )
		{
			//���յ������ݴ�С
		}
	}
	////////////////////////

	if (retVal >0)
	{
		if (m_bContinue)
		{ // һ��Json  û�����꣬ ��������
			if (m_nLength > m_nRecvLen + retVal  + m_nPos)
			{
				memcpy(m_pszJson + m_nRecvLen, m_recvBuf, retVal);
				m_nRecvLen += retVal;
			}
			else
			{
				memcpy(m_pszJson + m_nRecvLen,m_recvBuf, m_nLength - m_nRecvLen);
				Json::Value val;
				Pack(val, m_pszJson);
				HandleJson(val);
				m_nTempLen = m_nRecvLen + retVal - m_nLength;
				memcpy(m_tempBuf, m_recvBuf + m_nLength - m_nRecvLen, m_nTempLen);
				m_bContinue = FALSE;
				m_nRecvLen = 0;
			}
		}
		else
		{
			//��ȡ��ǰ���յİ��ĳ��ȣ�ǰ�ĸ��ֽ�Ϊ����
			if (retVal + m_nPos < 4)
			{
				//m_nPos += retVal;
				m_nTempLen = m_nPos + retVal;
				memcpy(m_tempBuf, m_recvBuf, m_nTempLen);
			}
			else
			{
				ByteToInt(&m_nLength, m_recvBuf);
				//Ϊ��Ҫ���յ�json�������ڴ�ռ�
				if (!m_pszJson)
				{
					m_pszJson = new char[m_nLength + 1];
					memset(m_pszJson, 0, m_nLength + 1);
				}

				//С��MAX_BUF�����
				if (m_nLength <= retVal + m_nPos  -4)
				{
					memcpy(m_pszJson, m_recvBuf + 4,m_nLength);
					Json::Value val;
					Pack(val, m_pszJson);
					HandleJson(val);
					m_nTempLen = retVal + m_nPos -m_nLength -4;
					memcpy(m_tempBuf, m_recvBuf + m_nLength + 4, m_nTempLen);
				}
				else 
				{	//��δ������
					memcpy(m_pszJson + m_nRecvLen,  m_recvBuf + 4,  retVal + m_nPos -4 );
					m_nRecvLen  +=  retVal + m_nPos - 4;
					m_bContinue = TRUE;
				}
			}
		}


	}
	//////////////


#if 0

	if (m_bFinished)
	{	
		//��ʼһ���µİ��Ľ���
		//��ȡ������
		if(strlen(m_recvBuf) < 4)
		{
			//�������գ��������ʱ�䣬��ʱ��������
		}

		//��ȡ��ǰ���յİ��ĳ��ȣ�ǰ�ĸ��ֽ�Ϊ����
		ByteToInt(&m_nLength, m_recvBuf);

		//Ϊ��Ҫ���յ�json�������ڴ�ռ�
		m_pszJson = new char[m_nLength + 1];
		memset(m_pszJson, 0, m_nLength + 1);
		m_bFinished = FALSE;


		if (m_nLength <= retVal -4)
		{
			//��Ҫ���ճ���С��ʵ�ʽ��յ��ĳ���
			//��ʱ�����һ�����⣬ ����������ַ���Ӧ��������һ�εĽ��գ�
			//��Ҫ���ǵ��ǽ�������ַ��洢

			m_bFinished = TRUE;

			memcpy(m_pszJson,m_recvBuf+4, m_nLength);
			if (m_nLength < retVal -4)
			{
				//����յ��ֽڴ洢����ʱ������
				m_nTempLen = retVal - 4 - m_nLength;
				memcpy(m_tempBuf, m_recvBuf+4+m_nLength, m_nTempLen);
			}

			Json::Value val;
#ifdef _DEBUG
			PrintSendOrRecvStr(m_pszJson, m_nLength,  _T("recv message\n") );
#endif
			Pack(val, m_pszJson);
			HandleJson(val);
		}
		else
		{
			memcpy(m_pszJson,m_recvBuf+4, retVal-4);
			//��¼��һ�νӰ��ڻ���������λ��
			m_nPos = retVal-4;
		}
	}
	else
	{
		//�������գ�ֱ������Ϊֹ
		if (m_nPos + retVal > m_nLength)
			memcpy(m_pszJson+m_nPos, m_recvBuf, m_nLength - m_nPos);
		//memcpy(m_tempBuf, m_recvBuf+m_nLength - m_nPos +1, m_nTempLen);
		else
			memcpy(m_pszJson+m_nPos, m_recvBuf, retVal);

		m_nPos += retVal;

		if (m_nPos >= m_nLength)
		{
			m_bFinished = TRUE;

			//˵���Ѿ��������m_nPos > m_nLength
			if (m_nPos > m_nLength)
			{
				//����յ��ֽڴ洢����ʱ���������´δ˽����ȶ�ȡm_tempBuf�� ע��ǰ�ĸ��ֽ��ǳ���
				m_nTempLen = m_nPos - m_nLength ;   // ������ֽڳ���

				// ��ʱ���ܴ���һ������ǰһ������һ��Json�Ľ�β�� ��һ������һ��Json��ͷ
				//memcpy(m_tempBuf, m_recvBuf+4+retVal, m_nTempLen);
				memcpy(m_tempBuf, m_recvBuf + retVal -  m_nTempLen , m_nTempLen);
				ByteToInt( &m_nLength, m_recvBuf + retVal -  m_nTempLen);
				//m_bFinished = FALSE;
				m_nPos = 0;
			}

			Json::Value val;
#ifdef _DEBUG
			//PrintSendOrRecvStr(m_pszJson, m_nLength,  _T("recv message") );
#endif
			Pack(val, m_pszJson);
			HandleJson(val);
		}
	}
#endif
}

#endif

//�ͻ��˷�������
void CIMSocket::SendData(const Json::Value& val)
{
	//���л����ڷ���
#if NOSERVICETEST
#else
	if (!m_bConnecting)
	{
		return;
	}
#endif
	

	//����Ϣ�ŵ���Ϣ����
	MakeMsgBuf(val, E_TOFRIEND); 
	//����Ϣ�ŵ���Ϣ����
	Json::FastWriter   writer;
	std::string  strOut = writer.write(val);
	 SendData(strOut);
}



void CIMSocket::SendData(const std::string & str)
{
	// m_serverType �����жϵ�ǰ���ӵķ�����

// 	int len = str.length();
// 	IntToByte(m_sendBuf, &len);
// 	memcpy(m_sendBuf+4, str.c_str(),len);
	char* pszVal = NULL;
	AnsiToUtf8(&pszVal, str.c_str());
	int len =  strlen(pszVal);
	IntToByte(m_sendBuf, &len);
	
#if 0
	int ret  = 0;
	memcpy(m_sendBuf+4, pszVal,len);

	ret = send(m_sock, m_sendBuf, len+4, 0);
#endif

#if 1
	int ret  = 0;
	if (len+4 <= MAX_SEND_BUF)
	{
		memcpy(m_sendBuf+4, pszVal,len);
		ret = send(m_sock, m_sendBuf, len+4, 0);
#ifdef _DEBUG

		PrintSendOrRecvStr(m_sendBuf+4, len, _T("send message"));
#endif
		
	}
	else
	{
		//��������������ѭ������
		int nCout =  (len + 4) / MAX_SEND_BUF;
		memcpy(m_sendBuf+4, pszVal,MAX_SEND_BUF-4);

#ifdef _DEBUG
		PrintSendOrRecvStr(m_sendBuf+4, MAX_SEND_BUF-4, _T("send message"));
#endif
		ret = send(m_sock, m_sendBuf, MAX_SEND_BUF, 0);

		for (int i=1; i<nCout; i++)
		{
			memcpy(m_sendBuf, pszVal+i*MAX_SEND_BUF -4, MAX_SEND_BUF);
#ifdef _DEBUG
			PrintSendOrRecvStr(m_sendBuf, MAX_SEND_BUF, _T("send message"));
#endif
			ret = send(m_sock, m_sendBuf, MAX_SEND_BUF, 0);
		}
		if ((len+4)%MAX_SEND_BUF != 0)
		{
			memcpy(m_sendBuf, pszVal+nCout*MAX_SEND_BUF-4, (len + 4)%MAX_SEND_BUF);
#ifdef _DEBUG
			PrintSendOrRecvStr(m_sendBuf, (len + 4)%MAX_SEND_BUF,_T("send message"));
#endif 
			ret = send(m_sock, m_sendBuf, (len + 4)%MAX_SEND_BUF, 0);
		}
	}
#endif
	

	if(SOCKET_ERROR == ret)
	{ 
		int nErrorCode =  WSAGetLastError();
		if (WSAEWOULDBLOCK == nErrorCode)
		{
			//��ʱ��������Ӧ�ý��д��� �ȴ�FD_WRITE
			OutputDebugString(_T("Send Block ,Please wait \n"));
		}
	}
	else
	{

	}
	SAFE_ARRYDELETE(pszVal);
}

void CIMSocket::SendLoginMsg(const Json::Value& val)
{
	if (m_bConnecting)
	{
		SendData(val);
	}
	else
	{
		Json::FastWriter   writer;
		m_strLoginJson = writer.write(val);
	}
}


//�ر�socket���յ���Ϣ
void CIMSocket::Close()
{
	//ExitSocket();
}

//�����յ��İ��ϲ���json��ʽ
void CIMSocket::Pack(Json::Value& val, const char*  str)
{
	Json::Reader reader;
	//Json::Value root;
	std::string strMsg;
	Utf8ToAnsi(strMsg, str);
	if (reader.parse(strMsg.c_str(), val))
	{
		//��ȡ��������
		OutputDebugString(_T("STRING TO JSON \n"));
#if _DEBUG
		std::string str1 = val[const_msgtype].asString();
		long  tmstamp = ((long long)(val["sendTime"].asDouble()))/1000;
		TCHAR tstr[24] = {0};
		TimestampToLocalTime(tstr, sizeof(tstr), tmstamp);
#endif 
	}
	SAFE_ARRYDELETE(m_pszJson);
}

/*
	��������
	���ܣ�
	���룺
	�����
	��������������ͻ��˽��յ�����Ϣjson
*/
void CIMSocket::HandleJson(const Json::Value& val)
{
	std::string &  strType = val[const_type].asString();
	//const char*  lpcStr = (char*)((val[const_type].asString()).c_str()); //ֱ����˵õ���ֵlpcStr�ǿ��ַ�����ԭ����
#ifdef _DEBUG
	Json::FastWriter writer;
	string strDeug   = writer.write(val);
	OutputDebugStringA((strDeug + "\n").c_str());
#endif

	if (strcmp(strType.c_str(), const_type_normal_msgval) == 0)
	{
		HandleNormalMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_login_responseval) == 0)
	{//��½����, ��ʼ������Ϣ������
		HandleLoginResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_logout_responseval) == 0)
	{//�˳�Ӧ��
		HandleLogoutResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), const_type_connect_responseval) == 0)
	{
		//������Ϣ����������
		HandleConnectResponseMsg(val);
	}
	else if (strcmp(strType.c_str(), "HeartbeatResponse") == 0)
	{
		HandleHeartBeatResponse(val);
	}
	else if (strcmp(strType.c_str(), "GetProfileResponse") == 0)
	{
		HandleProfileResponse(val);
	}
	else if (strcmp(strType.c_str(), "FriendGroup") == 0)
	{
		HandleFriendGroup(val);
	}
	else if(strcmp(strType.c_str(), "Offline") == 0)
	{
		//MessageBox(m_hWnd, _T("Offline"), _T("d"), MB_OK);
		HandleOfflineMsgs(val);
	}
	else if (strcmp(strType.c_str(), "Status") == 0)
	{
		HandleMemberStatus(val);
	}
	else if (strcmp(strType.c_str(), "RecentContacts") == 0)
	{
		HandleRecentContacts(val);
	}
}


//��ͨ��������Ϣ
void CIMSocket::HandleNormalMsg(const Json::Value& val)
{
	//���յ�������ͨ��������Ϣ
	//std::string strMsg = val["content"].asString();
	OutputDebugString(_T("Receive Normal message \n"));

	//���͵���Ӧ�Ĵ���
	//std::string strFriendId= val[const_fromid].asString().c_str();
	TCHAR* ptszId = NULL;
	C2T(&ptszId, val[const_fromid].asString().c_str());
	tString tstrFriendId = ptszId;
	SAFE_ARRYDELETE(ptszId);
	MAP_TSTRING_HWND::iterator it = g_mapWnd.find(tstrFriendId);

	//�ȴ洢
	MakeMsgBuf(val,E_FROMFRIEND);

	NormalMsg* pNormalMsg = new NormalMsg;
	pNormalMsg->FromJson(val);
	
	//���������ϵ���б�
	SendMessage(m_hWnd, WM_RECENTCONTACTS, 0 , (LPARAM)pNormalMsg);

	if (it == g_mapWnd.end())
	{
		//û�ҵ����ID����Ҫ���´���һ�����ڲ����
		//����ǿͷ��Ļ�ֻ��һ������������ڣ� ���Բ���Ҫ
		//���´�������
		//���ڿͷ��汾����������ǵ�һ�η������죬 ��ô��Ҫ�����������
		//��ϵ�����Ϻ󣬽���ϵ����ӵ������б�
		//g_mapWnd.insert(make_pair(tstrFriendId, g_mapWnd.at(tstrFriendId)));
	
		//HandleMsgOfDelay(val);
		SendMessage(m_hWnd, WM_CHATMSGWHENNOWND,  0, (LPARAM)pNormalMsg);
		return;
	}
	else
	
		//�ҵ���, �Ѿ����ھͲ��������
		//ֱ�ӽ���Ϣ����ȥ��
		//���յ�����ͨ��������Ϣ
		//MakeMsgBuf(val,E_FROMFRIEND);
		//��Ķ�Ӧ�Ĵ��ڷ�����Ϣ
		SendMessage(g_mapWnd[tstrFriendId], WM_REICEIVEMSG, 0, (LPARAM)(pNormalMsg));
	

	
}

//��¼��¼������ʱ��ķ��أ� ���ɹ�������Ϣ������
void CIMSocket::HandleLoginResponseMsg(const Json::Value& val)
{
	bool bSuccess = val["success"].asBool();
	LoginResponse loginResponse;
#ifdef  _DEBUG
	Json::FastWriter writer; 
	string strDebug = writer.write(val);
#endif
	loginResponse.FromJson(val);
	if (bSuccess)
	{
		const Json::Value&  servObj = val[const_allocatedMsgServ];
		std::string strServerAddr = servObj[const_host].asString();
		//m_strToken = val[const_token].asString();
		int port = servObj[const_port].asInt();
		//m_strLoginId = servObj[const_memberid].asString();
		m_connnectRequest.m_strToken = val[const_token].asString();
		m_connnectRequest.m_strLoginId = val[const_memberid].asString();

		closesocket(m_sock);
		//"10.0.40.18"
		InitSocket(E_MSGSERVER, loginResponse.m_nPort, loginResponse.m_strHost.c_str()/*"10.0.40.38"*/);
		//InitSocket(E_MSGSERVER, 17041, "10.0.40.10");
	}
	else
	{
		int nErrorCode =   loginResponse.m_nErrorCode;//val["errorCode" ].asInt();
		const string& strError =   loginResponse.m_strErrorMsg;//val["errorMessage"].asString();
		OutputDebugStringA(strError.c_str());
		tString tstrError ;

		if (nErrorCode == 1001)
			tstrError = _T("�û��������������");
		else if (nErrorCode == 1002)
			tstrError  = _T("�Ѿ���¼");
		TCHAR* lptError = NULL;
		C2T(&lptError, strError.c_str());
		SendMessage(g_mapWnd[g_loginwndKey],WM_ONLOGINERROR, 0, (LPARAM)tstrError.c_str());
		SAFE_ARRYDELETE(lptError);
#if _DEBUG

		TCHAR  tszError[48] = {0x0};
		wsprintf(tszError, _T("Error code: %d\n"), nErrorCode);
		OutputDebugString(tszError);
#endif
	}
}

void CIMSocket::HandleLogoutResponseMsg(const Json::Value & val)
{
	
}

//����������Ϣ������������Ϣ
//������Ϣ��������ǰ�û��Լ��Ļ�����Ϣ��self����
void CIMSocket::HandleConnectResponseMsg(const Json::Value& val)
{
	//ConnectResponse connectresponse;
	//connectresponse.FromJson(val);
	if (val["success"].asBool())
	{
		OutputDebugString(_T("LOGIN MSG SERVER SUCCEDD \n"));
		//֪ͨ��½�����Ѿ��Ѿ��ɹ���½
		
		//const Json::Value&  self = val["self"];
		g_user.InitUserInfoFromJson(val); 
		
		//CGlobalData::GetInstance()->m_recentLinkmen.FromJson(val); //�ͷ��汾���ص������ϵ����connect response ��
		//��¼ �ɹ�

		SendMessage(g_mapWnd[g_loginwndKey],WM_LOGINRET, TRUE, 0);
	//HandleOfflineMsgs(val);
	    PostMessage(m_hWnd,WM_SETUSERINFO, TRUE, 0);
		CSourceDownloadThread* pThread = new CSourceDownloadThread;
		SrcPara* pSrcPara = new SrcPara;
		pSrcPara->m_bFirstTimeToDownlod = TRUE;
		pSrcPara->m_hWnd = m_hWnd;
		pSrcPara->type = E_USERSELF;
		//pSrcPara->m_tstrId ;//_T("��Ʒ��"); //
		pThread->SetPara((UINT_PTR)pSrcPara);
		pThread->StartThread();
	}
}

//�����飬
void CIMSocket::HandleFriendGroup(const Json::Value& val)
{
	//�����鼰����Ϣ�洢��FriendList����ָ����
	FriendList::GetInstance()->FromFriendGroup(val);
	//����淢����Ϣ�ڽ�����ӵ�ǰ��
	tString tstrGroupName ;
	StringTotString( tstrGroupName, val["friendGroup"]["name"].asString());
	SendMessage(m_hWnd, WM_FRIENDGROUP, 0 ,(LPARAM)tstrGroupName.c_str());

	//�����̣߳��߳����ж��Ƿ���Ҫ���أ���Ҫ���صĻ��ڶ��߳��н�������
//#ifdef  _DEBUG
	CSourceDownloadThread* pThread = new CSourceDownloadThread;
	SrcPara* pSrcPara = new SrcPara;
	pSrcPara->m_bFirstTimeToDownlod = TRUE;
	pSrcPara->m_hWnd = m_hWnd;
	pSrcPara->type = E_NORMALMEMBER;
	pSrcPara->m_tstrId = tstrGroupName;//_T("��Ʒ��");
	pThread->SetPara((UINT_PTR)pSrcPara);
	pThread->StartThread();
//#endif
}

//�����յ�������Ϣ�� ��������Ϣ��
void CIMSocket::MakeMsgBuf(NormaMsgCollection* pMsgCollection)
{
	/*

	long  tmstamp = (long)(((long long)(val["sendTime"].asDouble()))/1000);
	char str[24] = {0x0};
	TimestampToLocalTime(str, 24, tmstamp);

	pMsgBug->datetime = str;
	pMsgBug->msgcontent = val[const_msgcontent].asString();
	if (fromorto == E_TOFRIEND)  //��ǰ�û�����ȥ����Ϣ
	{
	pMsgBug->id = val[const_toid].asString().c_str();
	}
	else //fromorto == E_TOFRIEND 
	{
	pMsgBug->id = val[const_fromid].asString().c_str();
	}
	pMsgBug->fromorto =  fromorto;
	pMsgBug->msgres = "";
	pMsgBug->msgid = 0;    //��ϢID�����ݿ���������
	*/
#if 1
	int nSize = pMsgCollection->m_vcNormalMsg.size();
	msginfo** MsgInfoArray = new msginfo*[nSize];
	int nIndex = 0;

	char strTime[24] = {0x0};
	for (vector<NormalMsg*>::iterator itMsg = pMsgCollection->m_vcNormalMsg.begin(); itMsg != pMsgCollection->m_vcNormalMsg.end(); ++itMsg, ++nIndex)
	{
	
 		msginfo* pMsg   = new msginfo;
  		TimestampToLocalTime(strTime, 23, (long)((*itMsg)->m_baseMsgInfo.m_dbSendTime/1000));
  
  		pMsg->datetime = strTime;
  		pMsg->id = (*itMsg)->m_strFromId;
  		pMsg->fromorto = E_FROMFRIEND;
  		pMsg->msgcontent = (*itMsg)->m_strContent;
  		pMsg->msgres = "";
  		pMsg->msgid = 0;//��ϢID�����ݿ���������
		pMsg->font = GetMsgfont();
  		MsgInfoArray[nIndex] = pMsg;
	}

	EnterCriticalSection(&g_sendcs);
	for (int i = 0; i < nSize; i++)
	{
		m_pThread->m_normalmsgQueue.push(MsgInfoArray[i]);
	}
	LeaveCriticalSection(&g_sendcs);
	
#endif
}

void CIMSocket::HandleOfflineMsgs(const Json::Value& val)
{
	COfflineMsgs & OfflineMsg = CGlobalData::GetInstance()->OfflinMessages();
	//CRecentLinkmen& Recentlinlmen = CGlobalData::GetInstance()->m_recentLinkmen;
	//�ȶ�ȡ������Ϣ
	//PostMessage()
#ifdef _DEBUG
 	Json::FastWriter writer;
 	string strDwug   = writer.write(val);
#endif
	OfflineMsg.FromJson(val);
	tString tstrId;
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = OfflineMsg.m_normalmsgMap.begin(); it != OfflineMsg.m_normalmsgMap.end(); )
	{
		MakeMsgBuf(it->second);
		StringTotString(tstrId, it->first);
		// �����������ϵ��
		SendMessage(m_hWnd, WM_RECENTCONTACTS, 0 , (LPARAM)(*(it->second->GetMessageCollection().rbegin())));
		//�����Ƿ����
		MAP_TSTRING_HWND::iterator itFind = g_mapWnd.find(tstrId);
		if (itFind != g_mapWnd.end())
		{
			//PostMessage()
			SendMessage(itFind->second, WM_REICEIVEMSG, 1, (LPARAM)(it->second));
			OfflineMsg.m_normalmsgMap.erase(it++);
		}
		else
			++it;
	}

	PostMessage(m_hWnd, WM_OFFLINEMESSAGES, 0, 0);
#if 0
	ProfileRequest profileRequest ;
	

	//����ʾ��Ϣ֮ǰ�� ���жϸ��û� �Ƿ���������ϵ�˵���
	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = OfflineMsg.m_normalmsgMap.begin(); it != OfflineMsg.m_normalmsgMap.end(); )
	{
		MAP_STRING_PLINKMANINFO::iterator itlinkman = Recentlinlmen.m_linkmenMap.find(it->first);
		
		//д������
		MakeMsgBuf(it->second);

		if (itlinkman != Recentlinlmen.m_linkmenMap.end())
		{
			// ��ϵ�˴���
			// ����Ϣ�ƽ�����ǰ����Ϣ��ȥͬ�¼ǵô����Ϣ��¼
			
			for (vector<NormalMsg*>::iterator itMsg = it->second->m_vcNormalMsg.begin(); itMsg != it->second->m_vcNormalMsg.end(); ++itMsg)
			{
				MsgOfArrival::GetInstance()->Insert(*itMsg);
			}
			OfflineMsg.m_normalmsgMap.erase(it++);
			
		}
		else
		{
			//��ϵ�˲�����
			//��Ϣ����������Ϣ������
			CMsgOfDelay::GetInstance()->m_normalmsgMap.insert(make_pair(it->first, it->second));
			profileRequest.m_vcId.push_back(it->first);
			OfflineMsg.m_normalmsgMap.erase(it++);
			//�������������ϵ����Ϣ
			Json::Value profileRequestJson;
			profileRequest.MakeJson(profileRequestJson , "GetProfileRequest", "GetProfileRequest");
			SendData(profileRequestJson);
		}
	}
#endif
}

void CIMSocket::HandleHeartBeatResponse(const Json::Value& val)
{
	if (val["success"].asBool())
	{
		int nSeq = val["seq"].asInt();
		if (nSeq != m_nReply+1)
		{
			//��������
			OutputDebugString(_T("HeartBeat Missing  error \n"));
		}
		m_nReply=nSeq;
	}
	else
	{
		int nErrorCode = val[cosnt_errorcode].asInt();
#if _DEBUG
		TCHAR tszError[128] = {0x0};
		wsprintf(tszError, _T("HeartBeat error , error code:%d\n"), nErrorCode);
		OutputDebugString(tszError);
#endif
	}
}

//�������������ϵ����Ϣ����
//��ɺ������촰�巢����Ϣ��ʼ��ʾ
//�̼Ұ���д�����
void CIMSocket::HandleProfileResponse(const Json::Value& val)
{
	const Json::Value& membersArry = val["members"];
	if (membersArry.isNull() || !membersArry.isArray())
	{
		OutputDebugString(_T("Method:HandleProfileResponse \n;  Error: member is null or not array \n"));
		//return;
	}

	//�浽�����ϵ����
	//CGlobalData::GetInstance()->m_recentLinkmen.FromJson(val);
	ProfileResponse::GetInstance()->FromJson(val);
	//�ͷ��汾��Ӧһ�����촰�����Զ�Ӧ�Ĵ��ھ������m_hwnd
	SendMessage(m_hWnd, WM_PROFILERESPONSE, 0, 0);
}

//ÿ�� ֻ����һ��Json., ����������Ĺ������У�������ֶ�������¼
//�� ��һ�����ӵ�ʱ�򷵻ص���������Ϣ���� ����������Ϣ
void CIMSocket::HandleMsgOfDelay(const Json::Value& val)
{
	NormalMsg*  pNormaMsg = new NormalMsg();
	pNormaMsg->FromJson(val);
	MAP_STRING_PNORMALMSGCOLLECTION& NormalMsgMap  = CMsgOfDelay::GetInstance()->m_normalmsgMap;
	//��ʱ�洢�ӳٵ���Ϣ���޸��û���Ϣ��¼�� ��Ҫ��ȡ��Ϣ������ʾ��
	if (NormalMsgMap.find(pNormaMsg->m_strFromId)!= NormalMsgMap.end())
	{
		NormalMsgMap.at(pNormaMsg->m_strFromId)->m_vcNormalMsg.push_back(pNormaMsg);
	}
	else
	{
		//�޸��û���Ϣ��¼
		NormaMsgCollection* pCollection = new NormaMsgCollection();
		pCollection->m_vcNormalMsg.push_back(pNormaMsg);
		NormalMsgMap.insert(make_pair(pNormaMsg->m_strFromId, pCollection));
	}
	//NormalMsgMap.insert(make_pair(pNormaMsg->m_strFromId, pNormaMsg));
	//�������������Ϣ��ȡ��ǰ�û���Ϣ
	ProfileRequest profileRequest;
	profileRequest.m_vcId.push_back(val[const_fromid].asString());

	Json::Value profileRequestJson;
	profileRequest.MakeJson(profileRequestJson, "GetProfileRequest", "GetProfileRequest");
	SendData(profileRequestJson);
}


void CIMSocket::HandleMemberStatus(const Json::Value& val)
{
 	#ifdef _DEBUG
 	Json::FastWriter writer;
 	string strDwug   = writer.write(val);
 	#endif
 	tString tstrMemberId;
 	int status;
 	StringTotString(tstrMemberId, val["memberId"].asString());
 	status = val["status"].asInt();
	SendMessage(m_hWnd, WM_SETMEMBERSTATUS, status, (LPARAM)tstrMemberId.c_str());
}

void CIMSocket::HandleRecentContacts(const Json::Value& val)
{
	CGlobalData::GetInstance()->RecentContacts().FromJson(val);
	SendMessage(m_hWnd, WM_RECENTCONTACTS, 0, 0);
}

//�������˲��õ��Ǵ�˴洢�ͷ��ͣ� �����ڴ��в��õ�

//��С�ˣ� ��Ҫ����ת��
void CIMSocket::ByteToInt(int* pVal,char* psz,int size )
{
	//int ��32λ����4���ֽ�
	ASSERT(size == 4);
	char* p = (char*)pVal;
	for (int i = 0; i < size; i++)
	{
		*(p+i) = *(psz+size-1-i);
	}
}

void CIMSocket::IntToByte(char* pDest, int* pSrc, int size /* = 4 */)
{
	//���������Ǵ�ˣ��ͻ�����С�ˣ�����int�ĸ�λ���ڵ͵�ַλ
	//
	ASSERT(size == 4);
	char* pSz = (char*)pSrc;
	for (int i = 0; i < size; i++)
	{
		*(pDest + i) = *(pSz+size-1-i);
	}
}

// 
// void CIMSocket::TimestampToLocalTime(WCHAR** dest, long tick)
// {
// //*dest = new WCHAR[24];
// // 	if (dest == NULL)
// // 	{
// // 		return ;
// // 	}
// // 	time_t tm_t = tick;
// // 	char str[24];
// // 	struct tm tm_;
// // 	tm_ = *localtime((time_t*)&tm_t);
// // 	
// // 	strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm_);
// 	
// 	char str[24];
// 	TimestampToLocalTime(str, 24, tick);
// 	C2W(dest, str);
// }
// 
// void CIMSocket::TimestampToLocalTime(char* dest, int len, long tick)
// {
// 	if (dest == NULL)
// 	{
// 		return ;
// 	}
// 	time_t tm_t = tick;
// 	//char str[24];
// 	struct tm tm_;
// 	tm_ = *localtime((time_t*)&tm_t);
// 	
// 	strftime(dest, len, "%Y-%m-%d %H:%M:%S", &tm_);
// 
// }


void CIMSocket::SetMsgfont(const CFontInfo& f)
{
	tStringToString(m_msgfont.name,  f.m_tstrName);
	m_msgfont.size  = f.m_nSize;
	m_msgfont.bold = f.m_bBold?true:false;
	m_msgfont.italics = f.m_bItalic?true:false;
	m_msgfont.underline = f.m_bUnderLine?true:false;
	m_msgfont.color = f.m_clrText;
}

msgfont& CIMSocket::GetMsgfont()
{
	return m_msgfont;
}