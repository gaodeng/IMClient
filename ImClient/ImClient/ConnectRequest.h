//////////////////////////////////////////////////////////////////////////
// class ConnectRequest
//	
// 
//
//
////
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "MsgData.h"

class ConnectRequest: public IMsgJson
{
public:
	ConnectRequest();
	~ConnectRequest();
public:
	std::string  m_strLoginId ;// ��¼
	std::string   m_strToken;  // tokenֵ
	//virtual void MakeJson(Json::Value& val);
	std::string   m_strProductType;
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) ;
};
