#include "StdAfx.h"
#include "MsgOfDelay.h"

CMsgOfDelay* CMsgOfDelay::m_pInstance = NULL;
CMsgOfDelay::CGarbo CMsgOfDelay::Garbo;
CMsgOfDelay::CMsgOfDelay(void)
{
}


CMsgOfDelay::~CMsgOfDelay(void)
{
	//ע�� ������Ϣ��Map��normalmsgָ�벻Ӧ�ñ�ɾ��
	//��map�е���Ϣ���ᱻ�Ƶ� MsgofAarry��
// 	for (MAP_STRING_PNORMALMSGCOLLECTION::iterator it = m_normalmsgMap.begin();
// 		it != m_normalmsgMap.end(); ++it)
// 	{
// 		NormaMsgCollection*  p = it->second;
// 		SAFE_DELETE(p);
// 	}
// 	m_normalmsgMap.clear();
	OutputDebugString(_T("CMsgOfDelay destructor \n"));
}

CMsgOfDelay* CMsgOfDelay::GetInstance()
{
	if (m_pInstance != NULL)
	{
		return m_pInstance;
	}
	m_pInstance = new CMsgOfDelay;
	return m_pInstance;
}
