#include "StdAfx.h"
#include "Path.h"

namespace ZYM
{

CPath::CPath(void)
{
}

CPath::~CPath(void)
{
}

// ��ȡӦ�ó���ִ��·��
tString CPath::GetAppPath()
{
	static TCHAR szPath[MAX_PATH] = {0};

	if (szPath[0] == _T('\0'))
	{
		::GetModuleFileName(NULL, szPath, MAX_PATH);

		tString strPath = GetDirectoryName(szPath);
		_tcsncpy(szPath, strPath.c_str(), MAX_PATH);
	}
	return szPath;
}

// ��ȡӦ�ó���ǰĿ¼
tString CPath::GetCurDir()
{
	TCHAR szCurDir[MAX_PATH] = {0};
	//::GetCurrentDirectory(MAX_PATH, szCurDir);

	//if (szCurDir[_tcslen(szCurDir) - 1] != _T('\\'))
	//	_tcscat(szCurDir, _T("\\"));

	_tcscat(szCurDir, CPaintManagerUI::GetInstancePath());
	return szCurDir;
}

// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
tString CPath::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = {0};

	::GetTempPath(MAX_PATH, szTempPath);

	if (szTempPath[_tcslen(szTempPath) - 1] != _T('\\'))
		_tcscat(szTempPath, _T("\\"));

	return szTempPath;
}

// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
tString CPath::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
}

// ��ȡ����ļ���(ȫ·��)
tString CPath::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	tString strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = {0};

	if (!IsDirectoryExist(lpszPath))
		strPath = GetCurDir();
	else
		strPath = lpszPath;

	strFileName = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (int i = 2; i < 10000; i++)
	{
		if (strExtFileName.empty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}
		
		if (!IsFileExist(strFullPath.c_str()))
			return strFullPath;
	}

	return _T("");
}

// ���ָ��·���Ƿ�Ŀ¼
BOOL CPath::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

// ���ָ���ļ��Ƿ����
BOOL CPath::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	if(::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)
		return TRUE;
	else
		return FALSE;
}

// ���ָ��Ŀ¼�Ƿ����
BOOL CPath::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);

	if((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

BOOL CPath::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR cPath[MAX_PATH] = {0};
	TCHAR cTmpPath[MAX_PATH] = {0};
	TCHAR * lpPos = NULL;
	TCHAR cTmp = _T('\0');

	if (NULL == lpPathName || NULL == *lpPathName)
		return FALSE;

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (int i = 0; i < (int)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
			cPath[i] = _T('/');
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (lpPos != NULL)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp = *lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}

	return TRUE;
}

// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
tString CPath::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.find(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// ����ָ��·���ַ�����Ŀ¼��Ϣ
tString CPath::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// ����ָ��·���ַ������ļ�������չ��
tString CPath::GetFileName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// ���ز�������չ����·���ַ������ļ���
tString CPath::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	tString::size_type nPos = strPath.rfind(_T('/'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	nPos = strPath.rfind(_T('.'));
	if (nPos != tString::npos)
		strPath = strPath.substr(0, nPos);

	return strPath;
}

// ����ָ����·���ַ�������չ��
tString CPath::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::size_type nPos = strPath.rfind(_T('.'));
	if (nPos != tString::npos)
		strPath = strPath.substr(nPos+1);

	return strPath;
}

// ����ָ�������·����ȡ����·��
tString CPath::GetFullPath(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL == *lpszPath)
		return _T("");

	tString strPath(lpszPath);

	tString::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') == *iter)
			*iter = _T('/');
	}

	TCHAR cFirstChar = strPath.at(0);
	TCHAR cSecondChar = strPath.at(1);

	if (cFirstChar == _T('/'))
	{
		tString strCurDir = GetAppPath();
		tString strRootPath = GetPathRoot(strCurDir.c_str());
		return strRootPath + strPath;
	}
	else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
	{
		return strPath;
	}
	else
	{
		tString strCurDir = GetAppPath();
		return strCurDir + strPath;
	}
}

}