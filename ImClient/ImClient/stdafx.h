// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_



// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
//objbase.h��װ�󲿷ֶ�com�Ĳ���
#include <objbase.h>
#include <vector>
#include <string> 
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#include <list>
#include<algorithm>
#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

#define MESSAGE_RICHEDIT_MAX  1024

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined
// Windows ͷ�ļ�:
#include <windows.h>

#define  IsStrEqual(str1, str2) ((_tcsicmp(str1, str2)==0)?1:0)
// C ����ʱͷ�ļ�
// #include <stdlib.h>
// #include <malloc.h>
// #include <memory.h>


//�ڴ��ͷź�
#define SAFE_ARRYDELETE(x) if(NULL!=x){delete[] x;x = NULL;}

#define SAFE_DELETE(x) if(NULL!=x){delete x; x =NULL;}

#include <tchar.h>

#include <UIlib.h>
using namespace DuiLib;


#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))
CDuiString GetCurrentTimeString();
namespace DuiLib {
// 	tString is a TCHAR std::string
// 		typedef std::basic_string<TCHAR> tString;
// #if defined(UNICODE) || defined(_UNICODE)
// 	typedef std::wstring tString;
// #else
// 	typedef std::string tString;
// #endif+
}

#define		FACE_CTRL_SEL		WM_USER + 1




#define  CUSTOMSERV  0

//class CIMSocket;
// 
// extern CIMSocket* G_IMSOCKET;
// 
// extern HWND     G_TESTWND ;

void W2C(char** dest, const WCHAR *src);

void C2W(WCHAR** dest, const char* src);

void C2T(TCHAR** dest, const char* src);

void T2C(char** dest, const TCHAR* src);

#define  MAX_FILENAME_SIZE 512   // �ļ��������ֵ

#define  WM_REICEIVEMSG			WM_USER + 2

#define  WM_PROFILERESPONSE     WM_USER + 3

#define  WM_SETHEADPICTURE      WM_USER + 4

#define  WM_TRAYMSG             WM_USER + 5

#define  WM_FRIENDGROUP         WM_USER + 6

#define  WM_SETEXISTINGPICTURE  WM_USER + 7     //ͷ����ͼ�������Ѿ����ڵ�ͼƬ

#define  WM_SETDOWNLOADPICTURE  WM_USER + 8		// ͷ����ͼ�� ���ز��������غ������ͼ
  
#define  WM_SETUSERINFO         WM_USER + 9 

#define  WM_OFFLINEMESSAGES     WM_USER + 10	 // ������Ϣ

#define  WM_CHATMSGWHENNOWND    WM_USER + 11     // �յ�������Ϣ����ʱû�����촰��

#define  WM_SETMEMBERSTATUS     WM_USER + 12     // ����member������״̬

#define  WM_RECENTCONTACTS      WM_USER + 13    // �����ϵ��

#define	 WM_RECVLINKMANINFO     WM_USER + 1022

#define  WM_LOGINRET			WM_USER + 1023

#define  WM_IMSOCKET			WM_USER + 1024 // 

#define  WM_SOCK_CONNECT		WM_USER + 1025 // 

#define  WM_SOCK_RWORCLOSE		WM_USER + 1026

#define  WM_ONLOGINERROR        WM_USER + 4000



#include <json/json.h>
#ifdef _DEBUG
#pragma  comment(lib,"jsonCpp_d.lib")
#else
#pragma  comment(lib,"jsonCpp_u.lib")
#endif

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
