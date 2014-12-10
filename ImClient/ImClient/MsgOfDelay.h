#pragma once

//#include "NormalMsg.h"

#include "NormaMsgCollection.h"
/************************************************************************/
/* 
	��ʱ��������Ϣ

	�����ڿͷ���Im 
	�����յ�һ����Ϣ��ʱ�� �������ϵ���б����沢û�е�ǰ���û�
	��ô��Ҫ���ݵ�ǰ��Ϣ���û���Id����ȡ�û�����Ϣ�� ����ǰ�û�
	��ӵ���ϵ���б�����ϵ���б�����ʾ�� �Ž���Ϣ��ʾ�����촰��
*/
/************************************************************************/
typedef std::map<std::string, NormalMsg*> MAP_STRING_PNORMALMSG ;



class CMsgOfDelay:public NormaMsgCollectionBase
{
public:
	~CMsgOfDelay(void);
	//MAP_STRING_PNORMALMSGCOLLECTION m_normalmsgMap;
	static CMsgOfDelay* GetInstance();
private:
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(CMsgOfDelay::m_pInstance);
		}
	};  
	static CGarbo Garbo;  //������Դ
	CMsgOfDelay(void);
	static CMsgOfDelay* m_pInstance;
};

