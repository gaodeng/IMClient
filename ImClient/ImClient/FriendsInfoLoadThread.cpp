#include "StdAfx.h"
#include "FriendsInfoLoadThread.h"
#include "FriendGroup.h"
CFriendsInfoLoadThread::CFriendsInfoLoadThread(void)
{
}


CFriendsInfoLoadThread::~CFriendsInfoLoadThread(void)
{
}

BOOL CFriendsInfoLoadThread::Run()
{
	FriendsInfoLoadThreadPara* p = (FriendsInfoLoadThreadPara*)(m_pPara);
	// �˴����Ը����û�����������������ݿ��н���ƥ��
    if (FriendList::GetInstance()->FromMsgdb())
	{
		PostMessage(p->m_hWnd, WM_LOGINRET, WPARAM(TRUE), 0);
	}
	return FALSE;
}