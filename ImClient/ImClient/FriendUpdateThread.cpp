#include "StdAfx.h"
#include "FriendUpdateThread.h"
#include "Sqlite.h"
#include "FriendGroup.h"
#include "Global.h"
#include "WOAMember.h"

CRITICAL_SECTION_EX CFriendUpdateThread::s_cs;

CFriendUpdateThread::CFriendUpdateThread(void)
{
}


CFriendUpdateThread::~CFriendUpdateThread(void)
{
}

//ע�⣺
//�ڶ��̸߳��±������ݿ⣬ �����������͵��Ǻ��Ѽ�����ı���� ��ô
//�ڴ˴�ֻ��Ҫ�����յ��ı�����µ��������ݿ⣬ ����Ŀǰ������ÿ�ζ���
//�������е���ϵ�ˡ�Ⱥ�����Դ˴�ÿ�ζ��Ὣ���յ���ϵ�˺�Ⱥ��洢��sqlite
//���ݿ���
BOOL CFriendUpdateThread::Run()
{
	//Sqlite Ŀ�ⲻ֧����һ���������ٿ���һ������

	s_cs.Lock();
	FriendGroup* p = (FriendGroup*)m_pPara;
	map<tString, BaseMember*>& memberMap = p->GetFriendGroupInfo().GetMemberMap();
	IMsgDb* pIMsgDb = CSqlite::GetIntstance()->GetMsgDbInterface();

	pIMsgDb->BeginTransaction();
	group g;
	g.id = p->GetFriendGroupInfo().m_strGroupId;
	g.name = p->GetFriendGroupInfo().m_strName;
	g.system = p->GetFriendGroupInfo().m_bSystem;
	pIMsgDb->AddGroup(g);
	friendsinfo f;
	// BaseMember* pMember = NULL;
	for (map<tString, BaseMember*>::iterator it = memberMap.begin(); it!=memberMap.end(); ++it)
	{
		//pMember = static_cast<WOAMember*>(it->second);
		it->second->GetFriendInfo(f);

// 		f.type = pMember->m_strType;
// 		f.membersource = pMember->m_strMemmberSource;
// 		f.id = pMember->m_strId;
// 		f.nickname = pMember->m_strNickName;
// 		f.sex = pMember->m_strSex;
// 		f.picurl = pMember->m_strPicUrl;
// 		f.signature = pMember->m_strSignature;
// 		f.tempmember = pMember->m_bTempmember;
// 		f.mobile = pMember->m_strMobile;
// 		f.telephone = pMember->m_strTelephone;
// 		f.address = pMember->m_strAddress;
// 		f.email = pMember->m_strEmail;
// 		f.init = pMember->m_bInit;
// 		f.post = pMember->m_strPost;
// 		f.department = pMember->m_strDepartment;
		pIMsgDb->AddFriend(f);
	}
	pIMsgDb->EndTransaction();
	s_cs.UnLock();

	delete this;
	return FALSE;
}