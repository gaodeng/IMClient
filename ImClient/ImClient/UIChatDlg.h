#pragma once
#include "UIFriends.hpp"
#include "UIFaceSelDlg.h"
#include "FontInfo.h"
#include "RichEditUtil.h"
#include "IMSocket.h"
#include "UIListCommonDefine.hpp"
enum E_PICSUFFIX
{
	E_PIC_GIF = 0,
	E_PIC_PNG,
	E_PIC_BMP,
	E_PIC_JPG,
	E_PIC_JPEG,
	E_PIC_EXIF,
	E_PIC_TIFF
};

enum E_FILEFORMAT
{
	E_PICTURE = 0,
	E_NORMALFILE
};

class InputViewEvent : public RichEditEvent
{
public:
	InputViewEvent(CRichEditUI *pReUI,const tString& bgimage, DWORD bkcolor);
	~InputViewEvent();
	//DragDrop
	virtual bool OnDragDropOver(const tString& pFilePath );
	virtual bool OnDropDown(const  tString& pFilePath,int nPos );
	virtual bool OnDropLeave(const tString& pFilePath );	
	/*---------------------------------------------------------------------*\
			decription : ������̵��ole����
			parament:
					pObj	���� �ļ�·�� �� ControlUI
					dwType	���� ֵΪ1ʱpObjָ���ļ�·����ֵΪ2ʱָ��ControlUI
					bRight	���� true��ʾ����Ҽ���false��ʾ������
	\*---------------------------------------------------------------------*/
	virtual void OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight );
	/*---------------------------------------------------------------------*\
			decription : ɾ��ole����
			parament:
					pObj	���� �ļ�·�� �� ControlUI
					dwType	���� ֵΪ1ʱpObjָ���ļ�·����ֵΪ2ʱָ��ControlUI
	\*---------------------------------------------------------------------*/
	virtual void OnDeleteOleObject(void* pObj,DWORD dwType);

	virtual void OnUrl(void* pObj,DWORD dwType,long lPos);
	 void GetFileType(const tString& strFilename, E_FILEFORMAT* format, int* suffix );
private:
	CRichEditUI *m_pReUI;
	tString bgimage_;
	DWORD	bkcolor_;
};

#if 0
class ShowViewEvent : public RichEditEvent
{
public:
	ShowViewEvent(CRichEditUI *pReUI,const tString& bgimage, DWORD bkcolor);
	~ShowViewEvent();
	//DragDrop
	virtual bool OnDragDropOver( const DuiLib::string& pFilePath );
	virtual bool OnDropDown( const DuiLib::string& pFilePath,int nPos );
	virtual bool OnDropLeave( const DuiLib::string& pFilePath );	
	/*---------------------------------------------------------------------*\
			decription : ������̵��ole����
			parament:
					pObj	���� �ļ�·�� �� ControlUI
					dwType	���� ֵΪ1ʱpObjָ���ļ�·����ֵΪ2ʱָ��ControlUI
					bRight	���� true��ʾ����Ҽ���false��ʾ������
	\*---------------------------------------------------------------------*/
	virtual void OnClickOleObject(void* pObj,DWORD dwType,long pt,bool bRight );
	/*---------------------------------------------------------------------*\
			decription : ɾ��ole����
			parament:
					pObj	���� �ļ�·�� �� ControlUI
					dwType	���� ֵΪ1ʱpObjָ���ļ�·����ֵΪ2ʱָ��ControlUI
	\*---------------------------------------------------------------------*/
	virtual void OnDeleteOleObject(void* pObj,DWORD dwType);

	virtual void OnUrl(void* pObj,DWORD dwType,long lPos);
private:
	CRichEditUI *m_pReUI;
	tString bgimage_;
	DWORD	bkcolor_;
};

#endif

struct  MsgHeaderFontInfo
{
	COLORREF nicknameClr; //�û��ǳ���ɫ
	COLORREF idClr;   //�û�id��ɫ
	COLORREF timeClr ; // ʱ����ɫ
	BOOL	 bHasUnderline;   //Id�Ƿ����»���
	BOOL     bIsLinked ;		//���ID�Ƿ��г�����
	int      fontSize;        //����Ĵ�С
	void SetMsgHeaderFontInfo(const MsgHeaderFontInfo & info)
	{
		nicknameClr = info.nicknameClr;
		idClr = info.idClr;
		timeClr = info.timeClr;
		bHasUnderline = info.bHasUnderline;
		bIsLinked = info.bIsLinked;
		fontSize = info.fontSize;
	}

} ;

typedef MsgHeaderFontInfo*  PMSGHEADERFONTINFO;

enum E_MSG_TYPE 
{
	MSG_NULL = -1,
	MSG_IN ,   //������Ϣ
	MSG_OUT      //������Ϣ
};

class CUIChatDlg:public WindowImplBase
{
public:
	
	//CUIChatDlg(const FriendListItemInfo& friend_info);
	CUIChatDlg(FriendListItemInfo* friend_info);
	~CUIChatDlg(void);
	virtual LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void Notify( TNotifyUI& msg );
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//�൱��MFC��OninitDialg���������ڵĳ�ʼ��
	virtual void  InitWindow();
	//void memberFxn( NMHDR * pNotifyStruct, LRESULT * result );
	static void OnNotifyLinkCallback(void*, CDuiString, LONG cpMin, LONG cpMax);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void    OnReceiveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT   OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	void SetOwner(HWND hwnd);
	HWND GetOwner();
protected:
	enum E_CHECKMSGRECORD
	{
		E_TORIGHT = 0,  // ���ұ߷�ҳ
		E_TOFIRSTPAGE , // ��һҳ ������Ϣ��¼
		E_TOLEFT,       // ���ұ߷�
		E_TOLASTPAGE    // ���һҳ�� �������Ϣ��¼
	};
	CUIChatDlg(void);
	void Init();
	void Msg2Json(Json::Value & val,const TCHAR*  lptsz);
	void OnPrepare(TNotifyUI& msg);
	void SendMsg();
	void HandleSendString(string& str);
	virtual void RecvMsg(NormalMsg* pNormMsg);
	//CDuiString GetCurrentTimeString();
	void OnFinalMessage(HWND hWnd);
	void OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnBtn_Emotion(TNotifyUI& msg);
	void OnBtn_FontBar(TNotifyUI& msg);
	void OnBtn_ViewSwitch(TNotifyUI& msg);
	void OnBtn_MsgRecord(TNotifyUI& msg);
	void OnBtn_SendMsg(TNotifyUI& msg);
	virtual void OnBtn_Close(TNotifyUI& msg);
	void OnBtn_Bold(TNotifyUI& msg);
	void OnBtn_UnderLine(TNotifyUI& msg);
	void OnBtn_Italic(TNotifyUI& msg);
	void OnBtn_SendImage(TNotifyUI& msg);
	void OnCmb_SelChange_FontSize(TNotifyUI& msg);
	void OnCmb_Selchange_FontName(TNotifyUI& msg);
	void OnBtn_Color(TNotifyUI& msg);
	void _RichEdit_ReplaceSel(ITextServices * pTextServices, CRichEditUI * pRichEdit, LPCTSTR lpszNewText, const msgfont* pMsgFont = NULL);
	void _RichEdit_GetText(ITextServices * pTextServices, tString& strText);	
	void ShowMsgInRecvEdit(const NormalMsg* pMsg, E_MSG_TYPE type);
	void ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type,  const NormalMsg* pMsg);
	void ShowMsgHeader(ITextServices *pTextServices, E_MSG_TYPE type,  const msginfo&   msg);
	void ShowMsgHeader(ITextServices* pTextServices, E_MSG_TYPE type, LPCTSTR lptTime);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  const NormalMsg* pMsg);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  LPCTSTR lptszText, const msgfont* pMsgFont = NULL);
	void AddMsg(ITextServices *pTextServices, CRichEditUI* pRichEdit,  const msginfo& msg);
	BOOL HandleSysFaceId(ITextServices *pTextServices, CRichEditUI * pRichEdit, LPCTSTR& p, tString& strText, const msgfont* pMsgFont = NULL);
	void SetMsgHeaderFontInfo(const MsgHeaderFontInfo & info);
	//����������ҡ����һҳ����һҳ��ť��ʾ��Ϣ��¼
	/*virtual*/ void ShowMsgRecord(E_CHECKMSGRECORD type);  
	// ��������������ʾ��Ϣ��¼
	void ShowMsgRecord(const char* date);
	void ShowOneMsgRecord(ITextServices* pTextServices, const msginfo& info);
	void SetWindowIcon();
	void StatrtFlashwindow();
	void StopFlashWindow();
	//��ʼ����Ϣ��¼�·�����һЩ��ť
	void InitMsgRecordController();   
	
	void SetEachPageRecordNum(int num);
	int  GetEachPageRecordNum();
//	void OnBtn_CheckMsgRecord(TNotifyUI& msg);
	void TurnMsgRecordPage(E_CHECKMSGRECORD type); 
	void SetTurnPageBtnState(bool bToFirstPage, bool bToLastPage);
	LRESULT OnDateChange(LPNMDATETIMECHANGE lpCHange);
protected:
	HWND               m_hMainFrame;
	FLASHWINFO          m_flashwInfo;
	//BOOL                m_bFocus;    // �����Ƿ����˽���
	//BOOL                m_bHasUnreadMsgAndFlash; // ��δ����Ϣ
	InputViewEvent*		m_pInputView;
	//ShowViewEvent*	m_pShowView;

	//tString bgimage_;
	//DWORD bkcolor_;
	//tString				m_tsrFriendId;  //��ǰ����ĺ��ѵ�ID 
	CFontInfo			 m_fontInfo;  //��ǰ���촰�ڵ�������Ϣ��
	CRichEditUI*		m_pSendEdit, * m_pRecvEdit, *m_pMsgRecordView;
	CComboUI*			m_pFontNameCmb, *m_pFontSizeCmb;
	DWORD			    m_dwLinkTextRange; //�û��г����ӵ�IM��
	FriendListItemInfo*  m_pFriendInfo;
	//FriendListItemInfo	m_friendInfo; //������Ϣ5
	BOOL				m_bShowMsgView;		//��Ϣ��¼���Ƿ����أ�״̬
	BOOL                m_bHideMsgView;		//���ص��Ƿ�����Ϣ��¼��������
	int					m_nExtendedWidth;   //������չ�������ӵĿ��
	SIZE				m_MinInfo;
	MsgHeaderFontInfo   m_msginFontinfo;

	MsgHeaderFontInfo   m_msgoutFontinfo;

	CUIFaceSelDlg		m_FaceDlg ;  //�����
	CFaceList			m_FaceList;  //����������
	BOOL				m_bDraged;    //���ڱ���ק
	IDataObject*		m_pIDragObj; // ��קԴ

	//std::string         m_strMsgRecordStartDate;
	// ÿһҳ����Ϣ��¼����ʾ������
	int                 m_nEachPageRecordNum;
	// �鿴��Ϣ��¼�ķ�ʽ�� ��ҳ
	E_CHECKMSGRECORD    m_checkMsgRecordType;
	// �����¼�Ƿ񵽵�һҳ
	bool                m_bToFristPage;
	// �����¼�Ƿ����һҳ
	bool				m_bToLastPage;

	bool                m_bLessComparison  ;
	vector<msginfo>     m_vMsgRecrd;
	//��ǰ��Ϣ��¼���������
	int			m_nMaxMsgId;
	//��ǰ��Ϣ��¼����С����
	int 		m_nMinMsgId;
};

