#pragma once

#include <string>

#if defined(UNICODE) || defined(_UNICODE)
// typedef std::wstring tString;
// #else
// typedef std::string tString;
#endif

// 2012.4.26 modify 

namespace ZYM
{

	class CPath
	{
	public:
		CPath(void);
		~CPath(void);

		static tString GetAppPath();	// ��ȡӦ�ó���ִ��·��
		static tString GetCurDir();	// ��ȡӦ�ó���ǰĿ¼
		static tString GetTempPath();	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
		static tString GetTempFileName(LPCTSTR lpszFileName);	// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
		static tString GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// ��ȡ����ļ���(ȫ·��)

		static BOOL IsDirectory(LPCTSTR lpszPath);	// ���ָ��·���Ƿ�Ŀ¼
		static BOOL IsFileExist(LPCTSTR lpszFileName);	// ���ָ���ļ��Ƿ����
		static BOOL IsDirectoryExist(LPCTSTR lpszPath);	// ���ָ��Ŀ¼�Ƿ����
		static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

		static tString GetPathRoot(LPCTSTR lpszPath);	// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
		static tString GetDirectoryName(LPCTSTR lpszPath);	// ����ָ��·���ַ�����Ŀ¼��Ϣ
		static tString GetFileName(LPCTSTR lpszPath); // ����ָ��·���ַ������ļ�������չ��
		static tString GetFileNameWithoutExtension(LPCTSTR lpszPath);	// ���ز�������չ����·���ַ������ļ���
		static tString GetExtension(LPCTSTR lpszPath);	// ����ָ����·���ַ�������չ��
		static tString GetFullPath(LPCTSTR lpszPath);	// ����ָ�������·����ȡ����·��
	};

}