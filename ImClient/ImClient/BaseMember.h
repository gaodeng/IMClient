#pragma once
//��ϵ�˻���ʹ�ü̳���ϵ��������չ
#include "..\\DataStorage\LocalData.h"

#define  OAMEMBERSRC "OA"
#define  WCNMEMBERSRC "WCN"
#define  TEMPMEMBERSRC "TEMP"
#define  BASEMEMBERSRC  "BASE"
class IMember
{
	virtual void FromJson(const Json::Value& val) = 0;
};

//friendsinfo�ṹ����һ�����ѵ���Ϣ�� ������OA�û�����WCN�û���Ϣ������������ṹ��
// ���Գ����DataStorage�����ݽṹ
class BaseMember:public IMember
{
public:
	BaseMember(void);
	virtual ~BaseMember(void);
	virtual LPCSTR GetMemberType();
public:

	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();
	//void SetTag_(UINT_PTR tag_);
	//UINT_PTR GetTag_();
	virtual void FromJson(const Json::Value& val);
	//�����ݿ���Ӧ�ṹ��ȡ
	virtual void FromFriendInfo(const friendsinfo& f);
	//ת��Ϊ��Ӧ��friendsinfo�ṹ
	virtual void GetFriendInfo(friendsinfo& f);
	std::string  m_strType;
	std::string m_strMemmberSource;
	std::string m_strId;
	std::string m_strNickName;
	std::string m_strSex;
	std::string m_strPicUrl;
	std::string m_strSignature;
	bool		 m_bTempmember;
	std::string m_strMobile;
	std::string m_strTelephone;
	std::string m_strAddress;
	std::string m_strEmail;
	bool        m_bInit;
protected: 	
	UINT_PTR m_pTag;
};

