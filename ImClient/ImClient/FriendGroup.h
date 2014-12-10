#pragma once
#include "BaseMember.h"
#include <map>
//////////////////////////////////////////////////////////////////////////
// class FriendGroup ������
//
//  ���������͵�Ⱥ��ֻ������ ����-Ա��
class FriendGroupInfo
{
public:
	FriendGroupInfo();
	~FriendGroupInfo();
	std::string m_strGroupId;
	std::string m_strName;
	bool                m_bSystem;
	void FromContactsArrayJson(const Json::Value& val);
	map<tString, BaseMember*>& GetMemberMap(){return m_mapContracter;}
	//UINT_PTR GetTag();
	//void SetTag(UINT_PTR tag);
private:	
	map<tString, BaseMember*> m_mapContracter; 
	UINT_PTR        m_pTag;  // ÿ�����Ѷ�Ӧ�Ľڵ�ָ��
};

//������ 
class FriendGroup
{
public:
	FriendGroup(void);
	~FriendGroup(void);
	FriendGroupInfo& GetFriendGroupInfo();
	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();
private:
	UINT_PTR           m_pTag;
	FriendGroupInfo    m_friendGroupInfo;
};

class FriendList
{
public:
	~FriendList();

	static FriendList* GetInstance();
	void FromFriendGroup(const Json::Value& val);
	map<tString, FriendGroup*> & GetFriendList();
	map<tString, BaseMember*>& GetMemberMap(const tString tstrId);
	BOOL FromMsgdb();
private:
	//<group���� ��>
	map<tString, FriendGroup*>  m_mapFriendList;
	FriendList();
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(FriendList::m_pInstance);
		}
	};
	static FriendList*  m_pInstance;
};