#pragma once
//////////////////////////////////////////////////////////////////////////
// 
// class LoginRequest 
//    ��¼��¼���������ݽṹ
//   
//
//
//////////////////////////////////////////////////////////////////////////

#include "MsgData.h"
class LoginRequest:public IMsgJson
{
public:
	LoginRequest();
	~LoginRequest();
public:
	std::string m_strLoginId; //��½ID
	std::string m_strPswd;   //��½����
	std::string m_strProductType; // ��Ʒ���� 
	//virtual void MakeJson(Json::Value& val);
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype)  ;
};