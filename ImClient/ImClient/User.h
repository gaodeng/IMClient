#pragma once
#include "..\\DataStorage\LocalData.h"
#include "linkmaninfo.h"
#include "ConnectResponse.h"

typedef ConnectResponse*  SELF;
/*
	�������ݿ���������������ݿ���Ʋ�һ��
	���·��ͻ��߽�����Ϣ��ʱ���ڴ洢�ڱ��ص�
	�����У���Ҫת��
*/
class CUser
{
public:
	CUser(void);
	~CUser(void);
public:
	BaseMember*	         UerSelf();
	userinfo			 m_userInfo;   //����Sqlite���ݿ�
	//userconfig			 m_userConfig; //����sqlite���ݿ�	
	linkmaninfo          m_selfInfo;   //�û�������Ϣ   // ���ڿͷ��汾


	void SetValue(const userinfo& val1, const userconfig& val2);
	void  InitUserInfoFromJson(const Json::Value val);  
private:
	ConnectResponse*   m_pConnResponse;
};

