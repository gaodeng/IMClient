#pragma once

/*
	�������Ϣ�� ������ʾ�ڽ����ϵ���Ϣ

	���ڿͷ��汾����


*/
#include "NormaMsgCollection.h"

class MsgOfArrival:public NormaMsgCollectionBase
{
public:
	~MsgOfArrival(void);
	static MsgOfArrival* GetInstance();
private:
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			OutputDebugString(_T("MsgOfArrival release \n"));
			SAFE_DELETE(MsgOfArrival::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //������Դ
	MsgOfArrival(void);
	static MsgOfArrival* m_pInstance;
};

