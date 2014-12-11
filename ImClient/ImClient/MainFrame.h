#ifndef	 MAINFRAME_HPP_
#define  MAINFRAME_HPP_ 
#include "UIFriends.hpp"
#include "UISysSetDlg.h"
#include "IMSocket.h"
#include "LogoutRequest.h"
#include "shellapi.h"
#include "FriendGroup.h"
#include  <hash_map>
#include  <list>
//#include "UIListCommonDefine.hpp"
#include "UIFriends.hpp"
#include "FriendUpdateThread.h"
class CMainFrame: public WindowImplBase
{
public:	
	//��Ӧ������ǩѡ��
	enum E_OPTION
	{
		NONE =0,
		FRIENDS,
		RECENTCONTACTS,
		APPLICATION
	};
	struct TabState
	{
		E_OPTION select ;
		E_OPTION mousevoer;
	};
	CMainFrame::CMainFrame(void);

	CMainFrame::~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFile();
	void UpdateGroup(FriendGroup* pGroup);
	void UpdateFriendsList();  //���º����б�
	void UpdateRecentList(NormalMsg* pMsg = NULL); //���������ϵ���б�
	virtual CDuiString GetSkinFolder();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND  Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle,CIMSocket *sock);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual LRESULT OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void       InitWindow();
	LRESULT OnTrayMsg(WPARAM wParam , LPARAM lParam);
	LRESULT  OnTimer(WPARAM wParam, LPARAM lParam);
	LRESULT  OnFriendGoup(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetExistingPicture(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetDownloadPicture(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetUserInfo(WPARAM wParam, LPARAM lParam);
	LRESULT  OnOfflineMessages(WPARAM wParam, LPARAM lParam);
	LRESULT  OnChatMessagesOfNoWnd(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetMemberStatus(WPARAM wParam, LPARAM lParam);
	LRESULT  OnSetUserStatus(WPARAM wParam, LPARAM lParam);
	LRESULT  OnRecentContacts(WPARAM wParam, LPARAM lParam);

	// Use zip Resource 
  	UILIB_RESOURCETYPE GetResourceType() const
  	{
  		return UILIB_ZIP;
  	}
  
  	virtual CDuiString GetZIPFileName() const
  	{
  		return _T("skin.zip");
  	}
protected:
	void Close();
	void Notify(TNotifyUI& msg);
	void OnTab_SelChanged(TNotifyUI& msg);
	void OnWindowInit(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void HideSerchEdit(CControlUI* pEdit);
	void OnBtn_SearchTip(TNotifyUI& msg);
	void OnBtn_SysSet(TNotifyUI& msg);
	void OnBtn_Signature(TNotifyUI& msg);
	void OnBtn_UserStatus(TNotifyUI& msg);
	void OnKillFocus_SerchEdit(TNotifyUI& msg);
	void OnKillFocus_SignatureEdit(TNotifyUI& msg);
	void OnDbItemClick(TNotifyUI& msg);
	void OnLItemClick(TNotifyUI& msg);
	void OnRItemClick(TNotifyUI& msg);
	void OnReturn(TNotifyUI& msg);
	void OnMenu_Online(TNotifyUI& msg);
	void OnMenu_Leave(TNotifyUI& msg);
	void OnMenu_Busy(TNotifyUI& msg);
	void OnMenu_UnDisturb(TNotifyUI& msg);
	void OnMenu_Stealth(TNotifyUI& msg);
	void OnMenu_Offline(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg){}
	void OnExit(TNotifyUI& msg){Close();}
	void OnBtn_Close(TNotifyUI& msg);
	void OnBtn_Minimize(TNotifyUI& msg);
	void OnBtn_Serach(TNotifyUI& msg);
	void OnTextChanged(TNotifyUI& msg);
	void ToTray();
	void DeleteTray();
	void SetMemberLogo(Node* pNode/*, LPCTSTR lptPic*/);
	//�������̹����˸
	HICON   m_hTrayIcon;
	BOOL	m_bIsIcon;     
	void    TrayFlash();
	void    BeginFlash();
	void    EndFlash();
	void    OnTrayDbButtonDown();
	void    OnTrayRButtonDown();
	//////////////////////////////
	//tString GetGrayPicName(LPCTSTR lptNormalName);
	void SetUserHeadPicture(LPCTSTR lptPicName);
	void  SetFriendListItemPicture(LPCTSTR lptGroupId, map<tString, tString>& mapIdToPic);
	BOOL  ShowChatWindow(const tString&  tstrId, BOOL bSureOfNoWnd = FALSE);
	void UpdateRecentList(RecentContact& recentContact, int nIndex = -1);
	void UpdateRecentList(FriendListItemInfo* pItem);
	void Search(LPCTSTR lptStr );
	void SetSearchBtnState(BOOL bSeachState);
	void  DisplaySearchView(BOOL bShow);
protected:
	typedef hash_map<tString, FriendListItemInfo*>    MAP;
	//std::list<FriendListItemInfo> m_vcFriendInfo;  //keshan 
	/*std::hash_map<tString, FriendListItemInfo*>*/MAP m_FriendItemInfo; // ��Ӧ��ÿ���˵�Item��Ϣ
	FriendListItemInfo myself_info_;
	// �û�ͷ�� <ID, ͼƬ·��>
//	std::map<tString, tString> m_mapMemberPic;
	TabState				 m_tabState;    //ͨ������¼��Ĵ���ģ��tab�ؼ���ʵ��
	CUISysSetDlg*			 m_pUISyssetDlg;
	CIMSocket*				 m_pIMSock;     //
	NOTIFYICONDATA			 m_nid;        
	tString					 m_tstrUserPic;   //�û�ͷ��·��
	CButtonUI*				 m_pBtnUserHead ;
	CTextUI*				 m_pTxtNickName;
	NormaMsgCollectionBase   m_notReadMessages; //δ������Ϣ����
	list<std::string>        m_listActiveFriend; // ��ǰδ����Ϣ�ĺ���	
	std::map<tString, Node*> m_mapGroupNode;   // ÿ��������ڵ�
	CFriendsUI*				 m_pFriendsList ;    // �����б�
	CFriendsUI*				 m_pRecentContactsList;  // 
	int					     m_nGroupId ;           // �Զ����GroupId�� ����	����˳��		
	int						 m_nCurSel;          // ��ǰtab ��ǩ��Ӧ��ѡ��
	std::map<string , RecentContact*>	 m_mapRecentContatct;
	CEditUI*				 m_pSerchEdit;
	CVerticalLayoutUI*		 m_pMainview;
	CVerticalLayoutUI*		 m_pSearchView;

	//CFriendUpdateThread* m_pFriendUpdateThread;
	////////////////////////////////////////////
	// �洢���ѷ�����״��= 
	
};

#endif