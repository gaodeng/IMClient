#include "StdAfx.h"
#include "UILoginDlg.h"
#include "UIStatusMenu.h"
//#include "MsgData.h"
#include "LoginRequest.h"
#include "IMSocket.h"
//#include "Global.h"
//#include "PicDowloadThread.h"
#include "FriendsInfoLoadThread.h"
extern const char* const_type_login_requestval;
extern const char* const_msgtype_login_requestval ;
const  TCHAR* const_acountedit = _T("accountedit");
const  TCHAR* const_pswedit = _T("passwordedit");
//extern const char* const_


CComboItemElement::CComboItemElement()
{

}

CComboItemElement::~CComboItemElement()
{

}

//��дListContainerElement
void CComboItemElement::DoEvent(TEventUI& event)
{

 	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN )
 	{
 		if( IsEnabled() )
 		{
 			LPCTSTR tstr = event.pSender->GetName();
 	
 			
 			if (_tcsicmp(tstr, _T("deltebtn")) == 0)
 			{
				//���comboitem�еİ�ť��ɾ����ǽ��item��
				//��Ҫע�����SelectItem������رյ�ǰ�Ĵ��ڣ� 
				//�����ɾ����ť����ε���SelectItem�� ������
				//֮ǰ��Item��ָ�붼����Closeǰ�ͷŵ��� �ᵼ����Close
				//��Ϣ�����õ�itemָ����Ч����bug

 				CComboItemElement* pElement = (CComboItemElement*)(event.pSender->GetTag());
 				CComboUI* pCombo = (CComboUI*)(pElement->GetOwner());
				pCombo->GetComboWnd()->Close();
				//�������б����Ƴ�
  				pCombo->Remove(pElement);
				//�ӱ������ݿ����Ƴ����û�����Ϣ
				userinfo uInfo;
				ComboItemInfo* pComboInfo = (ComboItemInfo*)pElement->GetTag();
				tStringToString(uInfo.id, pComboInfo->tstrId);
				CSqlite::GetIntstance()->GetUserDbInterface()->DeleteUser(uInfo);
 			}
			else
			{
				Select();
				Invalidate();
			}
 		}
 		return;
 	}
	if( m_pOwner != NULL ) CListContainerElementUI::DoEvent(event);
}

LPCTSTR CComboItemElement::GetClass() const 
{
	return _T("ComboItmeElement");
}

LPVOID CComboItemElement::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("ComboItmeElement")) == 0)
		return static_cast<CComboItemElement*>(this);
	return CListContainerElementUI::GetInterface(pstrName);
}

//////////////////////////////////////////////////////////////////////////

class CComboItemCallback: public IDialogBuilderCallback
{
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ComboItmeElement")) == 0)
		{
			return new CComboItemElement;
		}
		return NULL;
	}
};


//////////////////////////////////////////////////////////////////////////

CUILoginDlg::CUILoginDlg()
{
	m_nTimeout = 0;
	m_pCombo = NULL;
}


CUILoginDlg::~CUILoginDlg(void)
{
	//��map��ɾ��
	g_mapWnd.erase(g_loginwndKey);
}

CControlUI* CUILoginDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("ComboItmeElement")) == 0)
	{
		return new CComboItemElement;
	}
	return NULL;
}

LRESULT CUILoginDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{    
	case WM_NCLBUTTONDOWN:
		lRes = OnNcButtonDown(uMsg, wParam, lParam, bHandled);break;
	case  WM_REICEIVEMSG:
		OnReceiveMsg( uMsg,  wParam, lParam);
		break;
	case WM_LOGINRET:
		OnLoginRet(wParam, lParam);
		break;
	case WM_TIMER:
		{
			return OnTimer(uMsg,  wParam, lParam);
			break;
		}
 	case WM_KEYDOWN:
			break;
	case WM_ONLOGINERROR:
		 return OnLoginError1(uMsg,wParam, lParam);
	default:
		bHandled = FALSE;
		break;
	}
	if( bHandled ) return lRes;
	//if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUILoginDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1)
	{
		m_nTimeout++;
		if (m_nTimeout >=3)
		{
			m_nTimeout = 0;
			KillTimer(m_hWnd, 1);
			SwitchView();
			G_IMSOCKET->ExitSocket();

			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("loginerror")));
			if (pLabel != NULL)
			{
				pLabel->SetText(_T("���ӳ�ʱ"));
			}
		}
	}
	return 0;
}


void CUILoginDlg::OnLoginRet(WPARAM wParam, LPARAM lParam)
{
	BOOL ret = (BOOL)wParam;
	if (ret)
	{
		KillTimer(m_hWnd, 1);
		//��½�ɹ�
#if NOSERVICETEST
		OutputDebugString(_T("Login Success \n"));
		Close();
			return;
#else
		g_user.m_userInfo.res = g_user.UerSelf()->m_strPicUrl;
		g_user.m_userInfo.nickname = g_user.UerSelf()->m_strNickName;
		g_user.m_userInfo.loginid = g_user.UerSelf()->m_strId;
		g_user.m_userInfo.sex = g_user.UerSelf()->m_strSex;
		CSqlite*   p = (CSqlite*)CSqlite::GetIntstance();
		//AddUser����û�о�����оͲ�����ӣ����Ǹ÷�������Ҫ����
		//��������ڲ�������һЩ�������ݿ�ĳ�ʼ������
		p->GetUserDbInterface()->AddUser(g_user.m_userInfo);
	
		OutputDebugString(_T("Login Success \n"));
		Close();
#endif
		
		//CSqlite::GetIntstance()->GetUserDbInterface()->AddUser(g_user.m_userInfo);
	}
}


void CUILoginDlg::OnReceiveMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RecvMsg((LPCSTR)lParam);
}

void CUILoginDlg::RecvMsg(LPCSTR lpcStr)
{
	TCHAR* wStr= NULL;
	C2T(&wStr, lpcStr);
	SAFE_ARRYDELETE(wStr);
	//TODO: �����صĴ�����Ϣ
}

static bool OnComboItemDelBtnEvent(void* event)
{
	if (((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN)
	{
		CControlUI* pDelBtn = ((TEventUI*)event)->pSender;
		
		if (pDelBtn != NULL)
		{
			CComboItemElement* pListElement = (CComboItemElement*)(pDelBtn->GetTag());

			if (pListElement != NULL)
			{
				TEventUI* tEvent = (TEventUI*)event;
		
				pListElement->DoEvent(*(TEventUI*)event);
				
			}
		}
	}
	else
	{
		//pDelBtn
	}
	return true;
}

//�����ݿ��ж�ȡ��½����ǰ�ͻ��˵��û���Ϣ������m_listItemInfo������

void CUILoginDlg::InitItemInfoList()
{
	IUserDb* pIUserDb =  CSqlite::GetIntstance()->GetUserDbInterface();
	vector<userinfo> v;
	pIUserDb->QueryUserInfo(v);

	TCHAR tszDir[MAX_PATH+1] = {0x0};
	
	ComboItemInfo item;
	for (vector<userinfo>::iterator it = v.begin(); it != v.end(); it++)
	{
		StringTotString(item.tstrId, it->loginid);
		StringTotString(item.tstrNickname, it->nickname);
		StringTotString(item.tstrPswd, it->pswd);
		memset(tszDir, 0x0, sizeof(tszDir));
		
		/*item.tstrPicPath = _T("");*/
		//GetLinkmanPicPath(item.tstrPicPath, item.tstrId.c_str(), tszDir);
		if (it->res == "") 
		{
			if (it->sex == "Female") 
			// Ů��
				item.tstrPicPath = _T("female.png");
			else

				item.tstrPicPath = _T("male.png");

		}
		else
		{ // ��Ϊ��
			tString tstrUrl ;
			StringTotString(tstrUrl, it->res);
			GetPicDir(tszDir,MAX_PATH+1, E_USERSELF, item.tstrId.c_str());
			GetMemberPicPath(item.tstrPicPath, tszDir, tstrUrl.c_str(), NULL);
		}
		m_listItemInfo.push_back(item);
	}
}

//�����б�ĳ�ʼ��,�����б�
void CUILoginDlg::IniCombo()
{
	CComboItemCallback  ItemCallback;
	m_pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("accountcombo")));
	if (!m_pCombo)
	{
		return;
	}
	CDialogBuilder  dlgBuilder;
	if (m_listItemInfo.size() <=0)
	{
		return;
	}
// 	list<ComboItemInfo>::iterator it = m_listItemInfo.begin();
// 	CComboItemElement* pFirstEle= static_cast<CComboItemElement*>(dlgBuilder.Create(_T("accountcomboitem.xml"),0, &ItemCallback, &m_PaintManager));
// 	//pFirstEle->SetFixedHeight(37);
// 	CButtonUI* pBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pFirstEle, _T("deltebtn")));
// 	pBtn->SetTag((UINT_PTR)pFirstEle);
// 	pBtn->OnEvent += MakeDelegate(&OnComboItemDelBtnEvent);
// 	CControlUI* pControlHeader = static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pFirstEle, _T("headpic")));
// 	if (it->tstrPicPath != _T(""))
// 	{
// 		pControlHeader->SetBkImage(it->tstrPicPath.c_str());
// 	}
// 	//pFirstEle->SetAttribute(_T("padding"), _T("\"0,1,1,0\""));
// 	pCombo->Add(pFirstEle);
// 	it++;
	for (list<ComboItemInfo>::iterator it = m_listItemInfo.begin();it != m_listItemInfo.end(); ++it)
	{
		//�ж�xml�ļ��Ƿ��Ѿ���ʼ��
		CComboItemElement* pEle = NULL;
		if( !dlgBuilder.GetMarkup()->IsValid() )
		{
			pEle = static_cast<CComboItemElement*>(dlgBuilder.Create(_T("accountcomboitem.xml"), (UINT)0, &ItemCallback, &m_PaintManager));

		}
		else 
		{//�Ѿ�������xml�ļ�
			pEle = static_cast<CComboItemElement*>(dlgBuilder.Create(&ItemCallback, &m_PaintManager));
		}
	
		CButtonUI* pBtn1  = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pEle, _T("deltebtn")));
		pEle->SetTag(UINT_PTR(&(*it))); //tagΪ��Ӧitem��ϵ����Ϣ�ṹ���ָ��
		pBtn1->SetTag((UINT_PTR)pEle);
		pBtn1->OnEvent += MakeDelegate(&OnComboItemDelBtnEvent);
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pEle, _T("nickname")));
		if (it->tstrNickname != _T(""))
			pLabel->SetText(it->tstrNickname.c_str());
		else
			pLabel->SetText(it->tstrId.c_str());

		CControlUI* pControlHeader = static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pEle, _T("headpic")));
		if (it->tstrPicPath != _T(""))
		{
			pControlHeader->SetBkImage(it->tstrPicPath.c_str());
		}
		m_pCombo->Add(pEle);
	}
	m_pCombo->SelectItem(0);
	//m_pPswdText->SetText()

	
}

void CUILoginDlg::InitWindow()
{
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_acountedit));
	if (pAccountEdit != NULL)
	{
		//pAccountEdit->SetFocus();
	}
	
	InitItemInfoList();
	IniCombo();
	m_pCombo->SetFocus();
}

//֪ͨ��Ϣ
void CUILoginDlg::Notify(TNotifyUI& msg)

{
	if (IsStrEqual(msg.sType, _T("windowinit")))
	{		
		OnPrepare(msg);
		return;
	}
	else if (IsStrEqual(msg.sType, _T("dropwndhidden")))
	{
		CControlUI* pControlUI = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("accoutinputcontainer")));
		if (pControlUI != NULL)
		{
			pControlUI->SetBkImage(_T("file='accountcombo1.png'"));
		}
	}
	
	else if (IsStrEqual(msg.sType, _T("itemselect")))
	{
		OnItemSelect(msg);
	}
	else if (IsStrEqual(msg.sType, _T("dropdown"))) //�˺�������ĵ����Ϣ
	{
		OnDropdown(msg);
	}
	else if (IsStrEqual(msg.sType, _T("setfocus")))
	{
		OnSetFocus(msg);
	}
	else if (IsStrEqual(msg.sType, _T("killfocus")))
	{
		OnKillFocus(msg);
	}
	else if (IsStrEqual(msg.sType, _T("click")))
	{
		OnBtnClick(msg);
	}
	else if (IsStrEqual(msg.sType, _T("online")))
	{
		Menu_Online(msg);
	}
	else if (IsStrEqual(msg.sType, _T("leave")))
	{
		Menu_Leave(msg);
	}
	else if (IsStrEqual(msg.sType, _T("busy")))
	{
		Menu_Busy(msg);
	}
	else if (IsStrEqual(msg.sType, _T("undisturb")))
	{
		Menu_UnDisturb(msg);
	}
	else if (IsStrEqual(msg.sType, _T("stealth")))
	{
		Menu_Stealth(msg);
	}
	else if (IsStrEqual(msg.sType, _T("offline")))
	{
		Menu_Offline(msg);
	}
	else if (IsStrEqual(msg.sType, _T("return")))
	{
		//LogIn();
		return;
	}
	__super::Notify(msg);
}

void CUILoginDlg::OnDropdown(TNotifyUI& msg)
{
	CComboUI* pCombo =  static_cast<CComboUI*>(msg.pSender);
	if (IsStrEqual(msg.pSender->GetName(), _T("accountcombo")))
	{
		CControlUI* pControlUI = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("accoutinputcontainer")));
		if (pControlUI != NULL)
				pControlUI->SetBkImage(_T("file='accountcombo3.png'"));
	}
}

void CUILoginDlg::OnItemSelect(TNotifyUI& msg)
{
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("accountedit")));
	CEditUI* pPswdEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_pswedit));

	//
	CComboItemElement* pEle = (CComboItemElement*)(m_pCombo->GetItemAt(m_pCombo->GetCurSel()));
	ComboItemInfo* p = (ComboItemInfo*)(pEle->GetTag());
	pEdit->SetText(p->tstrId.c_str());
  	if (pPswdEdit != NULL)
	{	
			pPswdEdit->SetFocus();
			
 			pPswdEdit->SetText(p->tstrPswd.c_str());
			int nStrLen = _tcslen(p->tstrPswd.c_str());
 
 			pPswdEdit->SetSel(nStrLen, nStrLen);
	}
}

void CUILoginDlg::OnSetFocus(TNotifyUI& msg)
{
#ifdef _DEBUG
	if (IsStrEqual(msg.pSender->GetName(), _T("accountedit")))
	{
		CControlUI* pControlUI = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("accoutinputcontainer")));
		if (pControlUI != NULL)
		{
			pControlUI->SetBkImage(_T("file='accountcombo2.png'"));
		}
	}
#endif
}

void CUILoginDlg::OnKillFocus(TNotifyUI& msg)
{
#ifdef _DEBUG

	if (IsStrEqual(msg.pSender->GetName(), _T("accountedit")) || IsStrEqual(msg.pSender->GetName(), _T("accountcombo")))
	{
		CControlUI* pControlUI = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("accoutinputcontainer")));
		if (pControlUI != NULL)
		{
			pControlUI->SetBkImage(_T("file='accountcombo1.png'"));
		}
	}
#endif
}

void CUILoginDlg::OnBtnClick(TNotifyUI& msg)
{
	if (_tcsicmp(msg.pSender->GetName(),_T("loginBtn")) == 0)
	{
		// 			if (!LogIn())
		// 			{
		// 				Close(); 
		// 				return;
		// 			}
		LogIn();
		//Close(); 

	}
	else if (IsStrEqual(msg.pSender->GetName(), _T("closebtn")))
	{
		::PostQuitMessage(0);
	}
	else if (IsStrEqual(msg.pSender->GetName(), _T("userstatusbtn")))
	{
		CUIStatusMenu* pMenu = new CUIStatusMenu();
		if( pMenu == NULL ) { return; }
		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		::ClientToScreen(*this, &pt);
		pMenu->Init(msg.pSender, pt);
	}
	else if (IsStrEqual(msg.pSender->GetName(), _T("cancelbtn")))
	{
		KillTimer(m_hWnd, 1);
		m_nTimeout = 0;
		SwitchView();
		G_IMSOCKET->ExitSocket();
	}
}

void CUILoginDlg::Menu_Online(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status1.png' dest='2,2,13,13'"));
		//pButtonUI->SetBkImage()
		pButtonUI->SetNormalImage(_T("file='icon_status1.png' dest='2,2,13,13'"));
	}
}

void CUILoginDlg::Menu_Leave(TNotifyUI& msg)
{

	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status2.png' dest='2,2,13,13'"));
		//pButtonUI->SetNormalImage(_T("file='icon_status2.png' dest='2,2,13,13'"));
	}
}

void CUILoginDlg::Menu_Busy(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status3.png' dest='2,2,13,13'"));
		//pButtonUI->SetNormalImage(_T("file='icon_status3.png' dest='2,2,13,13'"));
	}
}

void CUILoginDlg::Menu_UnDisturb(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status4.png' dest='2,2,13,13'"));
		pButtonUI->SetNormalImage(_T("file='icon_status4.png' dest='2,2,13,13'"));
	}
}

void CUILoginDlg::Menu_Stealth(TNotifyUI& msg)
{
	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status5.png' dest='2,2,13,13'"));
		pButtonUI->SetNormalImage(_T("file='icon_status5.png' dest='2,2,13,13'"));
	}
}

void CUILoginDlg::Menu_Offline(TNotifyUI& msg)
{

	CButtonUI* pButtonUI = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("userstatusbtn")));
	if (pButtonUI != NULL)
	{
		pButtonUI->SetBkImage(_T("file='icon_status6.png' dest='2,2,13,13'"));
		pButtonUI->SetNormalImage(_T("file='icon_status6.png' dest='2,2,13,13'"));
	}
}

BOOL CUILoginDlg::LogIn()
{
	g_mapWnd.insert(make_pair(g_loginwndKey, this->m_hWnd));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_acountedit));
	CEditUI* pPswdEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(const_pswedit));
	if (pAccountEdit == NULL || pPswdEdit == NULL)
	{
		return FALSE;
	}

	char* strId= NULL;  
	char* strPswd = NULL; 
	T2C(&strId, pAccountEdit->GetText().GetData());
	T2C(&strPswd,pPswdEdit->GetText().GetData());
	if (strlen(strId)==0 || strlen(strPswd)==0)
	{
		return FALSE;
	}
	CControlUI* pCancelBtn = m_PaintManager.FindControl(_T("cancelbtn"));
	pCancelBtn->SetFocus();

	Json::Value val;
	LoginRequest  loginRequest;
	//loginRequest.Init(const_type_login_requestval, const_msgtype_login_requestval);
	loginRequest.m_strLoginId = strId;
	loginRequest.m_strPswd = strPswd;
	//���ܵ�½���Ҫ���ñ��浱ǰ�û�

	g_user.m_userInfo.id = strId;        // �û�ID ��ü��ܴ���
	g_user.m_userInfo.pswd = strPswd;    //���� ��Ҫ���ܴ���


	loginRequest.MakeJson(val, const_type_login_requestval, const_msgtype_login_requestval);
	
#if NOSERVICETEST  // û�з����������ز���
	//SeDirectory(pAccountEdit->GetText().GetData());
	CSqlite::GetIntstance()->GetUserDbInterface()->AddUser(g_user.m_userInfo);
	CFriendsInfoLoadThread* pFriendsInfoLoadThread = new CFriendsInfoLoadThread;
	FriendsInfoLoadThreadPara* pPara = new FriendsInfoLoadThreadPara;
	pPara->m_hWnd = m_hWnd;
	pPara->m_strUserid = strId;
	pPara->m_strPswd = strPswd;
	pFriendsInfoLoadThread->SetPara((UINT_PTR)pPara);
	pFriendsInfoLoadThread->StartThread();
#else
	//���ӵ�½������
	G_IMSOCKET->InitLoginServer();
	G_IMSOCKET->SendLoginMsg(val);
#endif

	SetTimer(m_hWnd, 1, 1000, 0);
	SwitchView();
	//��ת
	//�˴�Ӧ���ڱ���
	SAFE_ARRYDELETE(strId);
	SAFE_ARRYDELETE(strPswd);
	return TRUE;
}

LRESULT CUILoginDlg::OnLoginError1(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CVerticalLayoutUI* pLoginView = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("loginview")));
	if (pLoginView == NULL)
	{
 		return 0;
	}

	if (!pLoginView->IsVisible())
	{
		KillTimer(m_hWnd, 1);
		m_nTimeout = 0;
		SwitchView();
	}

	LPCTSTR lptError = (LPCTSTR)lParam;

	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("loginerror")));
	if (pLabel != NULL)
	{
		pLabel->SetText(lptError);
	}
	return 0;
}

void CUILoginDlg::SwitchView()
{
	CVerticalLayoutUI* pLoginView = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("loginview")));
	CVerticalLayoutUI* pLoggingView = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("Loggingview")));
	if (pLoginView == NULL || pLoggingView == NULL)
	{
		OutputDebugString(_T("Can't find loginview or loggingview \n"));
		return;
	}
	pLoggingView->SetVisible(!pLoggingView->IsVisible());
	pLoginView->SetVisible(!pLoginView->IsVisible());
	

}

//һЩ��Ҫ�Ի���ߴ���Ŀؼ����Է��ڴ˴�����
void CUILoginDlg::OnPrepare(TNotifyUI& msg)
{

}

//����Enter��Esc��
LRESULT CUILoginDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) {
		if( wParam == VK_RETURN ) {
			LogIn();
			return true;
		}
		else if( wParam == VK_ESCAPE ) {
			return true;
		}

	}
	return CCaptionWnd::MessageHandler(uMsg, wParam, lParam, bHandled);
}
#if 0

LRESULT CUILoginDlg::OnNcButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CControlUI* pLogoControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("imlogo")));
	//�����ת�ƽ��㣬�������ڳ��˼�����ť�������֮�⣬ �����ط����Ƿǿͻ���
	//����հ׵ĵط�ԭ����ý���Ŀؼ�����KillFocus
	if (pLogoControl != NULL && !pLogoControl->IsFocused())
	{
		pLogoControl->SetFocus();
	}
	bHandled = FALSE; //�˴�Ҫ��Ϊfalse
	return 0;
}



LRESULT CUILoginDlg::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUILoginDlg::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUILoginDlg::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

//����MFC�е�OnNcHitTest�� ��д�ú�����ʹ�����������ڿ����϶���ʱ�򣬿ؼ���������Ч
LRESULT CUILoginDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);


	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (-1 == rcCaption.bottom)
	{
		rcCaption.bottom = rcClient.bottom;
	}

	//һЩ��̬�ġ��ؼ��� �� ControlUI  ContainerUI Label�ȵ���������ǿ����϶���
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = m_PaintManager.FindControl(pt);
		if (IsInStaticControl(pControl))
		{
			return HTCAPTION;
		}
	}


	return HTCLIENT;

}

//����MFC�е�OnSize
LRESULT CUILoginDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

//
BOOL CUILoginDlg::IsInStaticControl(CControlUI *pControl)
{
	BOOL bRet = FALSE;
	if (! pControl)
	{
		return bRet;
	}

	CDuiString strClassName;
	std::vector<CDuiString> vctStaticName;

	strClassName = pControl->GetClass();
	strClassName.MakeLower();
	vctStaticName.push_back(_T("controlui"));
	vctStaticName.push_back(_T("textui"));
	vctStaticName.push_back(_T("labelui"));
	vctStaticName.push_back(_T("containerui"));
	vctStaticName.push_back(_T("horizontallayoutui"));
	vctStaticName.push_back(_T("verticallayoutui"));
	vctStaticName.push_back(_T("tablayoutui"));
	vctStaticName.push_back(_T("childlayoutui"));
	vctStaticName.push_back(_T("dialoglayoutui"));

	std::vector<CDuiString>::iterator it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
	if (vctStaticName.end() != it)
	{
		CControlUI* pParent = pControl->GetParent();
		while (pParent)
		{
			strClassName = pParent->GetClass();
			strClassName.MakeLower();
			it = std::find(vctStaticName.begin(), vctStaticName.end(), strClassName);
			if (vctStaticName.end() == it)
			{
				return bRet;
			}

			pParent = pParent->GetParent();
		}

		bRet = TRUE;
	}

	return bRet;
}
#endif


LRESULT CUILoginDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}