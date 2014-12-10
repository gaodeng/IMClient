#pragma once
//#include <json/json.h>
#include  <set>
//#include "linkmaninfo.h"

/************************************************************************/
/* 
	 ���ڿͷ��汾�������ϵ���൱����ϵ��
	 ����OA�汾����˵�����ϵ��ֻ��
*/
/************************************************************************/
enum E_LASTMSGTYPE
{
	E_SENDOUT = 0, 
	E_RECEIVE 
};
// struct RecentContactData
// {
// 	std::string  m_strId;
// 	std::string  m_strContent;
// 	double       m_dbLastTime;
// 	E_LASTMSGTYPE m_eLastMsgTye;
// };

class RecentContact
{
public:
	RecentContact();
	~RecentContact();
	RecentContact(const RecentContact& obj);
public:
	//RecentContactData& data()  ;
	//void FromJson(const Json::Value&  val);
	bool operator==(const RecentContact& obj) const;
	bool operator!=(const RecentContact& obj) const;
	bool operator<(const RecentContact& obj) const;
	bool operator>(const RecentContact& obj) const;
	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();

	std::string  m_strId;
	std::string  m_strNickName;
	std::string  m_strContent;
	double       m_dbLastTime;
	E_LASTMSGTYPE m_eLastMsgTye;
	//RecentContactData  m_data;
private:

	UINT_PTR     m_pTag;
};

class RecentContactSortFun
{
public:
	RecentContactSortFun(){}
	~RecentContactSortFun(){}
	//set���ж�Ԫ���Ƿ���ȣ�
	//if(!(A<B || B<A))����A<B��B<A��Ϊ��ʱ��������ȡ�
	bool operator()(const RecentContact& a, const RecentContact& b) const
	{
		//return a.data().m_dbLastTime < b.data().m_dbLastTime;
		int n = strcmp(a.m_strId.c_str(), b.m_strId.c_str());
// 		int n = strcmp(a.m_strId.c_str(), b.m_strId.c_str() );
// 		if (n != 0)
// 		{
// 			if(a.m_dbLastTime != b.m_dbLastTime)
// 				return a.m_dbLastTime > b.m_dbLastTime;
// 			 if (a.m_dbLastTime == b.m_dbLastTime)
// 				return n < 0;
// 		}
// 		else 
// 		{
// 			return a.m_dbLastTime > b.m_dbLastTime;
// 		}
// 		if (a.m_dbLastTime > b.m_dbLastTime)
// 		{
// 			if (n == 0)
// 			{
// 				return false;
// 			}
// 			else
// 
// 				return true;
// 		}
// 		else if (a.m_dbLastTime == b.m_dbLastTime)
// 		{
// 			if (n  == 0)
// 				return false;
// 			else 
// 				return n<0;
// 		}
// 		else
// 		{
// 			if (n == 0)
// 			{
// 				return false;
// 			}
// 			else 
// 				return false;
// 		}

		if (a.m_dbLastTime > b.m_dbLastTime)
		{
			return true;
		}
		else if (a.m_dbLastTime == b.m_dbLastTime)
		{
			return n<0;
		}
		else 
		{
			return false;
		}
	}
};

typedef std::set<RecentContact, RecentContactSortFun> RecentContactSet;

class RecentContactCollection
{
public:
	RecentContactCollection(void);
	~RecentContactCollection(void);
	//MAP_STRING_PLINKMANINFO  m_linkmenMap;

	void  FromJson(const Json::Value& val);
	void Erase(const std::string& str);
	RecentContactSet& RecentContacts();
private:
	RecentContactSet m_setRecentContacts;
// 	void Clear();
// 	void Erase(const std::string&  id);
// 	void Add(const linkmaninfo& info);
};

