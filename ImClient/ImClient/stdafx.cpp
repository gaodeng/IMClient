// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// ImClient.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ
#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
CDuiString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
	_stprintf_s( szTime, MAX_PATH, _T("\t%04d��%02d��%02d�� %02d:%02d:%02d\n"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return szTime;
}

//���ֽ�ת��Ϊ���ֽ�
void C2W(WCHAR** dest, const char* src)
{
	if (src == NULL)
	{
		return ;
	}

	size_t alen = strlen(src) + 1;
	size_t  ulen = (size_t)MultiByteToWideChar(CP_ACP, 0, src,alen,NULL, 0 )+1;

	*dest = new WCHAR[ulen];
	::MultiByteToWideChar(CP_ACP, 0, src, alen, *dest, ulen);
}

//���ֽ�ת��Ϊ���ֽ�
void W2C(char** dest, const WCHAR *src)
{
	if(src == NULL)
		return ;
	size_t len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0 , NULL, NULL);
	if (len == 0)
	{
		return;
	}
	*dest = new char[len];
	WideCharToMultiByte( CP_ACP, 0, src, -1, *dest, len, NULL, NULL );

}


void C2T(TCHAR** dest, const char* src)
{
#ifdef _UNICODE
	C2W(dest, src);
#else 
	//���ֽ�TCHAR���ǡ������
	int len = strlen(src)+1;
	*dest = new char[len];
	strcpy(*dest, src);
#endif
}

void T2C(char** dest, const TCHAR* src)
{
#ifdef _UNICODE
	W2C(dest, src);
#else
	int len = _tcslen(src) + 1;
	*dest = new TCHAR[len];
	strcpy(*dest, src);
#endif
}
// CIMSocket* G_IMSOCKET = NULL;

//HWND     G_TESTWND = NULL;