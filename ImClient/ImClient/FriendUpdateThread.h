#pragma once
#include "Thread.h"
/*
	Ŀǰ������޷�����ÿ�ζ����ͺ����б�ı��������ÿ�ζ����������еĺ�����Ϣ��

	��������ʵ����ÿ�η��ͺ��ѱ����ôCFriendUpdateThread��ÿ��Ҳֻ��Ҫ���ݷ���˷��͵ı���ı䱾�����ݿ⼴�ɡ�

	ĿǰCFriendUpdateThreadÿ�ζ��洢����˷��͵����е���ϵ��
*/
class CFriendUpdateThread:public CThread
{
public:
	CFriendUpdateThread(void);
	~CFriendUpdateThread(void);
private:
	virtual BOOL Run();
	static CRITICAL_SECTION_EX s_cs;
};

