#pragma once
#include "MsgData.h"

/************************************************************************/
/* 
	���ܣ� ��ϵ����Ϣ����

*/
/************************************************************************/
class ProfileRequest:public IMsgJson
{
public:
	ProfileRequest(void);
	~ProfileRequest(void);
	void AddBaseMsgInfoToJson(Json::Value& val);
public:
	//��ϵ�˵�Id����
	std::vector<std::string> m_vcId;
	virtual void MakeJson(Json::Value& val, const std::string& strtype, const std::string& strMsgtype) ;
	void FromJson(const Json::Value& val);
};

