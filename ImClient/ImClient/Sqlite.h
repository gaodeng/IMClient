#pragma once
#include "..\\DataStorage\LocalData.h"

#ifdef _DEBUG
#pragma comment(lib,  "DataStorage_d.lib")
#else
#   ifdef _UNICODE
#pragma comment(lib,  "DataStorage_u.lib")
#   endif
#endif

/*
	sqlite �ڶ��̲߳�����ʱ����ܳ����쳣�� ����ʹ���˵���ģʽֻ����һ��sqlite3�ṹָ�룬
*/

class CSqlite
{
private:
	CSqlite();
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(CSqlite::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //������Դ
public:
	~CSqlite(void);
	static CSqlite* GetIntstance();
	IUserDb* GetUserDbInterface() ;
	IMsgDb*  GetMsgDbInterface() ; 
	IIni*  GetIniInterface();
private:
	static  CSqlite*  m_pInstance;
	IUserDb*		       m_pIUserDb;
	IMsgDb*				   m_pIMsgDb;
	IIni*				   m_pIIni;
};

