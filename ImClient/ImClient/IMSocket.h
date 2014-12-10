#pragma once

#include "Global.h"
#define CLIENT_DLL_REEOR	0x01	//����Windows socket dllʧ��
#define CLIENT_EXIT_OK		0x0		//�ͻ��������˳�
#define MAX_NUM_BUF			1024		//���ջ���������󳤶�
#define MAX_SEND_BUF		1024
#define SERVERPORT			17041	//5556////ͨ�Ŷ˿�
#define SERVERIP			/*"127.0.0.1"	*/"10.0.41.102"
// #define SERVERPORT			5556////ͨ�Ŷ˿�
// #define SERVERIP			"127.0.0.1"
//#include <json/json.h>
#include "FontInfo.h" 
#include "DBWriterThread.h"
#include "HeartBeatThread.h"
//#include "FriendUpdateThread.h"
#include<WinSock2.h>
#include "ConnectRequest.h"
#pragma comment(lib, "ws2_32.lib")

void HandleString(std::string & str);

class CIMSocket
{
	
public:
	CIMSocket(void);
	~CIMSocket(void);
	void Create(HWND hwnd);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	enum E_SERVERTPE
	{
		E_NOSERVERCONNECT = -1,
		E_LOGINSERVER,
		E_MSGSERVER
	};
	enum E_FROMORTOTYPE
	{
		E_FROMFRIEND = 0,   //�յ� ��Ϣ
		E_TOFRIEND			//���͵���ǰ���û�
	};

private:
	SOCKET  m_sock;			//�ͻ�������SOCKET
	HWND    m_hWnd;     //��Ϣ���մ��ھ��
	
	CHAR	m_sendBuf[MAX_SEND_BUF]; //���ͻ�����
	CHAR    m_recvBuf[MAX_NUM_BUF];  //���ջ�����
	CHAR    m_tempBuf[MAX_NUM_BUF]; // ��ʱ������,�洢һ��Json������������ں��������ֽ�
	int     m_nTempLen;				//��ʱ�������洢���ֽڳ���
	BOOL    m_bConnecting;		//�Ƿ�����
	BOOL    m_bFinished;       // ����һ�����Ƿ����
	int     m_nLength;			 //������
	int     m_nPos;				//�ϰ�����һ�κϰ�������λ��
	char*	m_pszJson;    //���յ���json��
	E_SERVERTPE m_serverType;  //��Ϣ����������
	//std::string m_strToken; //
	CDBWriterThread*  m_pThread; //д��Ϣ�߳���ָ��
	CHeartBeatThread*  m_pHeartBeatThread;  // �����߳�
	//CFriendUpdateThread* m_pFriendUpdateThread;
	int					m_nReply;  //��������
	std::string  m_strLoginJson;    //��½json 
	char			m_szLoginServerIp[24];
	int          m_nLoginPort;  
	//std::string   m_strLoginId; // Ϊlogin Response ���ص�memberid 
	ConnectRequest m_connnectRequest;
	int m_nRecvLen; // �Ѿ����յ��ĳ���
	BOOL m_bContinue ;  // һ��Json�ֶ�ν����Ƿ����
public:
	void SendData(const Json::Value& val);
	void SendData(const std::string & str);

	BOOL InitLoginServer();
	void  SendLoginMsg(const Json::Value& val);
	void ExitSocket(void);

	void SetMsgfont(const CFontInfo& f);
private:
	
	BOOL InitSocket(E_SERVERTPE type, int port = SERVERPORT, LPCSTR lpcStr = SERVERIP);
	BOOL InitMsgSock(int port, const char* strip);
	void InitDBWirterThread();
	void InitHeartBeatThread();
	void ReceiveData(void);
	void HandleStickPackeage(BOOL bWhenNewRecv = TRUE);
	void Close(void);
	void Pack(Json::Value& val, const char*  str);
	void HandleJson(const Json::Value& val);
	void HandleNormalMsg(const Json::Value& val);
	void HandleLoginResponseMsg(const Json::Value& val);
	void HandleLogoutResponseMsg(const Json::Value & val);
	void HandleConnectResponseMsg(const Json::Value& val);
	//void HandleSelfInfo(const J)
	void HandleHeartBeatResponse(const Json::Value& val);
	void HandleMsgOfDelay(const Json::Value& val);
	void HandleProfileResponse(const Json::Value& val);
	void HandleOfflineMsgs(const Json::Value& val);
	void HandleFriendGroup(const Json::Value& val);
	void HandleMemberStatus(const Json::Value& val);
	void HandleRecentContacts(const Json::Value& val);
	inline void ByteToInt(int* pVal,char* psz, int size = 4); 
	inline void IntToByte(char* pDest, int* pSrc, int size = 4);
	//��ʱ�������ȷ���룩תΪ��ǰ��ʱ���ַ���
	//void TimestampToLocalTime(WCHAR** dest, long tick);
	//void TimestampToLocalTime(char* dest, int len,  long tick);
	void MakeMsgBuf(const Json::Value & val, E_FROMORTOTYPE fromorto);
	void MakeMsgBuf(NormaMsgCollection* pMsgCollection);

	msgfont   m_msgfont;
	msgfont& GetMsgfont();
};

