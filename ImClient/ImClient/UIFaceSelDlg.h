#pragma once
#include "FaceCtrl.h"


class CUIFaceSelDlg: public WindowImplBase
{
public:
	CUIFaceSelDlg(void);
	~CUIFaceSelDlg(void);
public:
	LPCTSTR GetWindowClassName() const;	
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	void SetFaceList(CFaceList * lpFaceList);
	int GetSelFaceId();
	int GetSelFaceIndex();
	tString GetSelFaceFileName();

protected:
	void Notify(TNotifyUI& msg);

private:
	CFaceList * m_lpFaceList;
	int m_nSelFaceId;  //��Ӧxml�е�id
	int m_nSelFaceIndex; //��Ӧ���鴰�ڵ�����
	tString m_strSelFaceFileName;
	tString m_tstrFaceTips;
	CFaceCtrl * m_pFaceCtrl;
};

