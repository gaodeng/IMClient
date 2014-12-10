// ImClient.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ImClient.h"
#include "MainFrame.h"
#include "UILoginDlg.h"
//#include "UIEnterpriseWnd.h"
/*#include "IMSocket.h"*/
#define MAX_LOADSTRING 100
#include "Update.h"
// ȫ�ֱ���:
//HINSTANCE hInst;								// ��ǰʵ��
//TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
//TCHAR szWindowClass[MAX_LOADSTRING];			// ����������


//�������
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE /*hPrevInstance*/,
                     LPTSTR    /*lpCmdLine*/,
                     int       nCmdShow)
{
	//GdiplusStartupInput   gdiplusStartupInput;
	//ULONG_PTR               gdiplusToken;
	//	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CUpdate update; 
	// ����Ƿ���Ҫ����
	//update.CheckUpdate();
	if(update.CheckUpdate())
		return 0;
	

	HINSTANCE hInstRich = ::LoadLibrary(_T("Riched20.dll"));
	::CoInitialize(NULL);
	::OleInitialize(NULL);

	CPaintManagerUI::SetInstance(hInstance);
	
	//source in files
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	
	
	// Use zip Resource
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	//CPaintManagerUI::SetResourceZip( _T("skin.zip"));

#if CUSTOMSERV
	CUIEnterpriseWnd *pEnterprisDlg = new CUIEnterpriseWnd;
	if (pEnterprisDlg == NULL)
	{
		return 0;
	}
	pEnterprisDlg->Create(NULL, _T("�ͷ�"), WS_OVERLAPPEDWINDOW/*UI_WNDSTYLE_FRAME*/,WS_EX_WINDOWEDGE,new CIMSocket());
	pEnterprisDlg->CenterWindow();
#else
	CMainFrame  mainFrame;
	mainFrame.Create(NULL, _T("ImClient"), WS_OVERLAPPEDWINDOW/*UI_WNDSTYLE_FRAME*/, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW/*| WS_EX_TOPMOST*/, new CIMSocket());
	mainFrame.CenterWindow();
#endif					
 	CUILoginDlg/*CUISysSetDlg */loginDlg;
 	//��Ҫע��FRAME����DIALOG���
 	//loginDlg.Create(NULL, _T("Login"), UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
 	HWND hwnd = loginDlg.Create(NULL, _T("��¼"), UI_WNDSTYLE_DIALOG | WS_EX_APPWINDOW, 0);
	loginDlg.SetIcon(IDI_IMCLIENT);
 	loginDlg.CenterWindow();
 	int ret = loginDlg.ShowModal();
#if CUSTOMSERV
	pEnterprisDlg->SetIcon(IDI_IMCLIENT);
	
	::ShowWindow(*pEnterprisDlg, SW_SHOW);
	pEnterprisDlg->InitWindow();
#else 
	mainFrame.ShowWindow(true);
#endif
	//
	
	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	::OleUninitialize();
	::CoUninitialize();
	::FreeLibrary(hInstRich);
	//GdiplusShutdown(gdiplusToken);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
	//SetTitle(IDI_IMCLIENT);
//	SetIcon(IDI_IMCLIENT, TRUE);
#endif
	return 0;
}


