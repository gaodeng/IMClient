#include "StdAfx.h"
#include "Update.h"
#include "Sqlite.h"
#include "Global.h"
#pragma comment (lib, "Version.lib")   

typedef bool (*http_downloadtobuf) (const char* , char*, int);


CUpdate::CUpdate(void)
{
	//int x =  sizeof(m_tszVersion);
	memset(m_tszVersion, 0,  sizeof(m_tszVersion));
}


CUpdate::~CUpdate(void)
{
}

BOOL CUpdate::GetFileVersion(TCHAR* lptVersion, int size)
{
	VS_FIXEDFILEINFO * pVerInfo = NULL;
	DWORD dwTemp, dwSize, dwHandle = 0;
	BYTE* pData = NULL;
	UINT uLen;

	TCHAR filename[MAX_PATH] = {0x0};
	GetModuleFileName(NULL, filename, MAX_PATH); 

	dwSize = GetFileVersionInfoSize(filename, &dwTemp);
	if (dwSize ==0 )
		return FALSE;

	pData = new BYTE[dwSize];
	if (pData == NULL)
		return FALSE;

	if (!GetFileVersionInfo(filename,dwHandle, dwSize, pData))
		return FALSE;
	if (!VerQueryValue(pData, _T("\\"), (void**)&pVerInfo, &uLen))
	{
		delete[] pData;
		return FALSE;
	}
	DWORD verMS = pVerInfo->dwFileVersionMS;
	DWORD verLS = pVerInfo->dwFileVersionLS;

	int ver[4];
	ver[0] = HIWORD(verMS);
	ver[1] = LOWORD(verMS);
	ver[2] = HIWORD(verLS);
	ver[3] = LOWORD(verLS);
	_stprintf_s(lptVersion,8, _T("%d.%d.%d.%d"), ver[0], ver[1], ver[2], ver[3]);
	delete[] pData;
	return TRUE;
}

BOOL CUpdate::CheckUpdate(const char* url, tString& tstrPackageUrl)
{
	BOOL bUpdate =FALSE;
	HINSTANCE hDll;
	http_downloadtobuf downloadtobuf;
#ifdef _DEBUG
	hDll = LoadLibrary(_T("HttpDownload_d.dll"));
#else
	hDll = LoadLibrary(_T("HttpDownload.dll"));	
#endif

	if (hDll == NULL)
	{
		return 0;
	}
	downloadtobuf = (http_downloadtobuf)GetProcAddress(hDll,"http_downloadtobuf");
	if (downloadtobuf == NULL)
	{
		FreeLibrary(hDll);
	}

	char buf[2048] = {0x0};
	if (!downloadtobuf(url, buf, 2048))
	{
		//����ʧ��
		OutputDebugString(_T("�ӷ�������ȡ�汾��Ϣʧ��"));
		FreeLibrary(hDll);
		return FALSE;
	}

	//buf �д洢ʵ�����Ǹ�xml�ĵ�
	TCHAR* lptXml  = NULL;
	C2T(&lptXml, buf);

	//��ȡ��ǰ�汾
	//TCHAR tszVer[10] = {0x0};
	GetFileVersion(m_tszVersion, 10);


	CXmlDocument xmlDoc;
	CXmlNode xmlUpdateNode , xmlSubNode;
	BOOL bRet = xmlDoc.LoadXml(lptXml);
	delete[] lptXml;

	if (!bRet)
	{
		OutputDebugString(_T("The return is not xml doc, maybe the url is wrong \n"));
		return FALSE;
	}
	if (xmlDoc.SelectSingleNode(_T("update "), xmlUpdateNode))
	{
		//BOOL bRet_;
		//bRet_ = xmlUpdateNode.SelectSingleNode(_T("version"), xmlSubNode);
		
		std::wstring wstrVer = xmlUpdateNode.GetAttribute(_T("version"));
		if (_wcsicmp(wstrVer.c_str(),m_tszVersion) > 0)
		{
			//��Ҫ����
			bUpdate = TRUE;
		}
	
	}

	if (bUpdate)
	{
		
			tstrPackageUrl = xmlUpdateNode.GetAttribute(_T("url"));
		     if (tstrPackageUrl.empty())
				bUpdate  =FALSE;
	}
	FreeLibrary(hDll);
	return bUpdate;
}

/************************************************************************/
/* 
	������
	//http://update.myhouse.w.cn/api/v1/version.xml?product=im&version=1.0.0.2
	ע�⣺product=im
*/
/************************************************************************/
BOOL  CUpdate::CheckUpdate()
{
	IIni* pIni = CSqlite::GetIntstance()->GetIniInterface();
	TCHAR tszUrl[500] = {0x0};
	BOOL bRet =FALSE;
	bRet = pIni->ReadString(_T("update"), _T("url"), NULL, tszUrl, 500);
	if (!bRet)
		//�����ļ���
		return FALSE;
	std::string strUrl;
	tStringToString(strUrl, tszUrl);
	tString tstrPackageUrl;
	bRet =  CheckUpdate(strUrl.c_str(), tstrPackageUrl);
	if (bRet)
	{
		//�򿪸��½���tstrPackageUrl Ϊ�������
		TCHAR tszPara[512] = {0x0};
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFO siStartupInfo;
		memset(&siStartupInfo,0,sizeof(STARTUPINFO));
		siStartupInfo.cb = sizeof(STARTUPINFO);

		 ZeroMemory( &piProcInfo, sizeof(piProcInfo) );
		 /*
		 �˴���Ҫע��tszPara����ǰ����Ͽո�_tWinMain�����е�lpCmdLineΪȥ���ո���tszPara,������ǰ��
		 ���ӿո�lpCmdLineΪ��  GetCommandLine����Ӱ��
			*/
		_stprintf_s(tszPara,  _T(" %s?product=%s&version=%s"),tstrPackageUrl.c_str(),/*_T("househelper")*/_T("im") ,m_tszVersion/*_T("1.0.3")*/);
		TCHAR tszPath[MAX_FILENAME_SIZE]  = {0x0};
		GetModuleFileName(NULL, tszPath, MAX_FILENAME_SIZE-1); 
		(_tcsrchr(tszPath, _T('\\')))[0] = 0;//ɾ���ļ�����ֻ���·��
		_tcscat_s(tszPath,_T("\\update.exe"));
		_tcscat_s(tszPath, tszPara);
 		if (CreateProcess(NULL, tszPath, NULL, NULL, FALSE, 0, NULL, 0,&siStartupInfo, &piProcInfo ))
 		{
			DWORD dw = GetLastError();

 			CloseHandle(piProcInfo.hThread);
 			CloseHandle(piProcInfo.hProcess);
 			//exit(0);
			return TRUE;
 		}
	}
	return FALSE;
}