#pragma once
#include "Thread.h"
#include <map>
#include "Global.h"
#include  <set>

typedef std::map<std::string, tString>  MAP_STRING_TSTRING ;

typedef std::map<tString, tString>  MAP_TSTRING_TSTRING ;



#define  MUTITHREAD_DWOONLOAD_NUM  5 // ÿ���߳���������

//#define  MAX_WAITTHREAD       64  //�ȴ��߳������ֵ





class PicInfoEx
{
public:
	//�ڵ�ǰ����Ķ��member�У�����������ͷ�������ػ���δ����
	// <memberid, picUrl>
	MAP_TSTRING_TSTRING m_mapHasPic;  //��ͷ��������
	//MAP_TSTRING_TSTRING m_mapNotHasPic;  //��ͷ����Ҫ����
	std::map<tString, vector<tString>> m_mapPicToId;  //��Ҫ����ͷ���map<url, vector<����ID>>
};

// ͼƬ-����IDӳ��


//PicDownloadThreadEx �̲߳���
class PicDwonloadPara
{
public:
	PicDwonloadPara();
	~PicDwonloadPara();
	HWND m_hWnd;
	MAP_TSTRING_TSTRING m_mapDownInfo;
	std::map<tString, vector<tString>> m_mapPicInfo;
	TCHAR   m_tszDir[MAX_PATH + 1];  
	E_MEMBER_TYPE type;
	tString m_tstrGroupId;
};

//ͼƬ�����߳�
class PicDownloadThreadEx: public CThread
{
public:	
	PicDownloadThreadEx();
	~PicDownloadThreadEx();
	//HANDLE GetHandle(){return m_hThread;}
private:
	static CRITICAL_SECTION_EX s_cs;
	BOOL Run();
	BOOL URLDownloadPicture(LPCTSTR lptUrl , LPCTSTR  lptPicPath, HINSTANCE hDll);
};

//CSourceDownloadThread �̲߳���
class SrcPara
{
public:
	SrcPara();
	~SrcPara();

	tString     m_tstrId;  // GoupId or userself ID

	BOOL          m_bFirstTimeToDownlod;
	HWND          m_hWnd;

	E_MEMBER_TYPE type;

	///////
	PicInfoEx     m_picInfo;
};




//������������̣߳� ���ȴ����ؽ���
class CSourceDownloadThread:public CThread
{
public:
	CSourceDownloadThread();
	~CSourceDownloadThread(void);
private:
	BOOL IsMemberHasPic(BaseMember* pMember, LPCTSTR lptDir);
	void GetPicDownloadThreadPara(SrcPara* pSrcPara);
	BOOL Run();
	//����ͼƬ���·��
	std::vector<PicDownloadThreadEx*>       m_vThread;
	TCHAR     m_tszDir[MAX_PATH + 1];  
	//void GetPicDir(LPTSTR lptStr, int nSize, E_MEMBER_TYPE type,  LPCTSTR lptId= NULL);
	static CRITICAL_SECTION_EX s_cs;;
};

