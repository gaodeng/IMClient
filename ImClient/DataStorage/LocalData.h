#ifndef LOCALDATA_H
#define LOCALDATA_H
#define DB_OK 0
#define DB_FAILED 1
#define DB_EXIST  0x2

#define  USER_TYPE_WOA "woa"
#define  USER_TYPE_WCN "wcn"

#define NOSERVICETEST      1 //�޷���״̬�µ�������

#ifdef  DATASTORAGE_EXPORTS
#define DATADLL_API __declspec(dllexport)  
#else
#define DATADLL_API  __declspec(dllimport) 
#endif
//dll�нӿں�������ʹ����Vector����û��ʹ��map��һЩ��Ϊ���ӵ�stl�����ݽṹ����Ϊ����һЩ��Ϊ���ӵ����ݽṹ��Ϊ��ڻ��߳��ڲ������ܵ���ĳЩ�쳣

//�ڴ��ͷź�
#define SAFE_ARRYDELETE(x) if(NULL!=x){delete[] x;x = NULL;}

#define SAFE_DELETE(x) if(NULL!=x){delete x; x =NULL;}

//����Sqlite3�е�û�����Լ����������ɾ����ʱ�������
//����ӻص�����ʵ��
#ifdef DATASTORAGE_EXPORTS
//EXE����Ҫ����
#define null 0x0
//#include <string.h>
size_t __strlen(const char* str);

#include <sqlite3.h>
#include <Windows.h>
#include <tchar.h>

#ifdef _DEBUG
#pragma comment(lib,  "Sqlite3_d.lib")
#else
#   ifdef _UNICODE
#pragma comment(lib,  "Sqlite3_U.lib")
#   endif
#endif

#endif

typedef int DB_FLAG;
#define DB_USERINFO 0x01
#define DB_MSG      0x02

//�������е����ݿ�����ӿ�
//�ӿڰ������еı����ļ����ݿ����

#include <string>
#include <vector>
using namespace std;

#ifdef DATASTORAGE_EXPORTS

//void HandleString(string & str);

#endif

//�������ݽṹ��Ӧ��������

//��ǰ��¼������ͻ��˵��û�
typedef struct tag_userinfo
{
	string id;  //�û�Id Ψһ��ʾ��
	string nickname;
	int  logintimes;
	string lastlogintime; 
	string res;
	string type;
	string     sex;   // Female ����Male
    string loginid; //��¼id
	string  pswd;   // ��¼����
} userinfo,*PUSERINFO;

typedef struct tag_userconfig
{
	string id;
	short  loginstatus;
	short  savepswd;
	short  autologin;
	short  autostart;
	short  exitconfirm;
	short  autohide;
	short  showintaskbar;
	short  upgrademode;
	short  mergewnd;
	short  autoshowmsg;
	short  leavestatusset;
	string  defaultderectory;
	short  criticalfilesize;
}userconfig, *PUSERCFG;

typedef struct tag_group
{
	string id;
	string name;
	bool   system;
} group, *PGROUP;

//�˴�ֻ���ǵ���OA�û��� û�п���TEMP�û���WCN�û������ݽṹ���������
typedef struct tag_friendsinfo
{	
	string type;
	string membersource;
	string id;
	string nickname;
	string sex;
	string picurl;
	string signature;
	bool   tempmember;
	string mobile ;
	string telephone;
	string address;
	string email;
	bool   init;
	//OA 
	string post;
	string department;
	/*
	//WCN
	std::string   m_strContractor;
	std::string   m_strShopName;
	bool	      m_bMobilevalid;
	bool		  m_Realnamevalid;
	bool		  m_bMerchant;
	*/
}friendsinfo, *PFRIEND;;

typedef struct tag_msgfont
{
	string name;
	short  size;
	bool   bold;
	bool   italics;
	bool   underline;
	int    color;
} msgfont, *PMSGFONT;

typedef struct tag_msginfo 
{
	string  id;
	int msgid;
	short fromorto;
	string msgcontent;
	string msgres;
	string datetime;
	msgfont font;
}msginfo, *PMSGINFO;

enum e_msginfo_field
{
	e_msginfo_id = 0, 
	e_msginfo_msgid,
	e_msginfo_fromto,
	e_msginfo_msgcontent,
	e_msginfo_msgres,
	e_msginfo_date,
	e_msginfo_font 
};

enum e_comparison_operators
{
	e_biger = 0,
	e_less ,
	e_equal,
	e_bigerorequal,
	e_lessorequal
};

//userinfo.db�Ļ�������
class   IUserDb
{
public:
	IUserDb();
	virtual ~IUserDb();
	//�򿪻��ߴ����������ݿ�
	virtual int QueryUserInfo(vector<userinfo>& v) = 0;	
	virtual int AddUser(const userinfo& info)= 0;
	virtual int DeleteUser(const userinfo& info) =0;
	virtual int UpdateUser(const userinfo& info)=0;
	//virtual int QueryFriendsInfo(vector<friendsinfo>& v,const char* strId)=0;
	virtual int QueryCfgInfo(userconfig& cfginfo, const char* szId)=0;
	//���뱣֤���ô˺�����֮���ڵ������������Ա
	virtual void FinalRelease()=0 ; 
};

//Msg.db�� ��������
class IMsgDb
{
public:
	IMsgDb();
	virtual ~IMsgDb();
	virtual int BeginTransaction() = 0;
	virtual int EndTransaction()= 0;
	virtual int AddGroup(const group&	g) = 0;
	virtual int QueryGroupCollection(vector<group>& v) = 0;
	virtual int AddFriend(const friendsinfo& f) = 0;
	virtual int DeleteFriend(const friendsinfo& f) = 0;
	virtual int UpdateFriend(const friendsinfo& f) = 0;
	virtual int QueryFriendsCollection(vector<friendsinfo>& v, const string & groupid="", const string& groupname="") = 0;
	virtual int QueryMsg(msginfo& msg, const string & id, int msgid)=0;
//  virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id) = 0;
	virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id, int msgid, bool bLessComparison=true ,bool bAsc=false ,int num=50,int offset=0 ) = 0;
	virtual int QueryMsgCollection(vector<msginfo>& v, const string& id, const string& formDate,  const string & toDate ) = 0;
// 	virtual int AddTable(const string& id)=0;
// 	virtual int DropTable(const string& id)=0;
	virtual int AddMsg(const msginfo& msg, const string & id="")=0;
	virtual int DeleteMsg(int msgid, const string & id) =0;
	virtual int DeleteMsgCollection(int msgidfrom, int nCounts , const string & id ) = 0;
	virtual bool IsExist(const string& id, e_msginfo_field field, e_comparison_operators comparison, const string& value, int num = 1) = 0;
	virtual void FinalRelease()=0 ; 
};

class IIni
{
public:
	IIni();
	~IIni();
public:
	virtual void InitPath() = 0;
	virtual BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString) = 0;
	virtual DWORD ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR buff, DWORD nSize) = 0;
	virtual UINT ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault) = 0;
	virtual void SetPath(LPCTSTR lpPath) = 0;
	virtual void FinalRelease()=0 ; 
};

#ifdef DATASTORAGE_EXPORTS

//�ӿ�ʵ�����ݿ������ �ͷţ����ݱ�Ĵ���������sql������
//��ͬʱҪ��������exec������ʹ�����񣬴��ӿ�ִ���ٶ�
//���߳�ʹ��ȫ�־��ʱ�����뽨��pthread_mutex_t�����ǿ��Ա�֤���߳�֮��ִ�����ݿ�prepare_v2����ʱ����ͻ�����򣬵�һ���߳�prepare_v2�����pStmt���ڲ���ʱ����һ���߳�sqlite3_finalize(pStmt),��ô�ͻ�����쳣��������������down��
class  IDBInit
{
	//ע���û����Բ���
public:
	IDBInit();
	virtual ~IDBInit();
	int OpenDB(sqlite3** ppdb, DB_FLAG dbtype) ;
	virtual int CreateDB(sqlite3* pdb, DB_FLAG dbtype) ; 
	virtual int Init(DB_FLAG dbtype) = 0;

	virtual int ManipulateSql(sqlite3_stmt** ppstmt, const char* sql, bool bSelectSql = false);
	sqlite3* m_pConn;
	bool     m_bExist;		//��ǰ���ݿ������Ƿ����
	static char     strUid[30];	//��ǰ��¼�õ�ID�� Ҳ�Ƕ�Ӧ�ĸ���Ŀ¼��
};


//�ļ�·���ӿ�
class IPath
{
public:
	IPath();
	~IPath();
	static TCHAR s_tszPath[MAX_PATH]; //IM�ļ��洢·�������ֽ�
	static char  s_szPath[MAX_PATH*2]; //IM�ļ��洢·��,���ֽ�
	static int SetDirectory(LPCTSTR lptStr);  //��������IM�ͻ��˴洢·��
	static int SetUserDirectory(LPCTSTR lptStr, LPCTSTR lptType = NULL);
	//void	DeleteUserDirectory(LPCTSTR lptStr);
	BOOL  DeleteFolder(LPCTSTR lpstrFolder);
	//BOOL  DeleteFolder(LPCTSTR lpstrFolder);
	static BOOL   IsDirectoryExist(LPCTSTR lptStr);
private:
	//static BOOL   IsDirectoryExist(LPCTSTR lptStr);
};

//�����ļ�д����
class CIni:public IIni
{
public:
	CIni();
	~CIni();
public:
	virtual void InitPath();
	virtual BOOL WriteString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	virtual DWORD ReadString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR buff, DWORD nSize);
	virtual UINT ReadInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefault);
	virtual void SetPath(LPCTSTR lpPath);
	virtual void FinalRelease() ; 
private:
	TCHAR m_tszFilePath[MAX_PATH];
};

class UserDb:public IDBInit, public IUserDb, public IPath
{
public:
	UserDb();
	~UserDb();
public:
	virtual int Init(DB_FLAG dbtype) ;
public:
	//��ȡ���еĵ�ǰ�ͻ����û���Ϣ
	virtual int QueryUserInfo(vector<userinfo>& v) ;	
	//����û����û���������ӣ� �����ڲ���Ӹ��ݷ���ֵ�ж�
	virtual int AddUser(const userinfo& info);
	//ɾ��һ���û������������ݿ���ɾ��֮�⻹��Ҫɾ�����û��ı���Ŀ¼��
	virtual int DeleteUser(const userinfo& info);
	//����һ���û�
	virtual int UpdateUser(const userinfo& info);
	//��ȡ��ǰ�û��ĸ���������Ϣ
	virtual int QueryCfgInfo(userconfig& cfginfo,const char* szId);
	//��Դ���ͷ�
	bool IsUserExist(const char* szId);
	virtual void FinalRelease();
};

//������Ϣ�Լ������¼��������MsgDb����
class MsgDb:public IDBInit, public IMsgDb
{
public:	
	MsgDb();
	~MsgDb();
	virtual int Init(DB_FLAG dbtype) ;
	//BeginTransaction��EndTransaction��Ҫ���ʹ�ã�����ʧ�ܻ���лع�����
	virtual int BeginTransaction();
	virtual int EndTransaction();
	virtual int AddFriend(const friendsinfo& f);
	virtual int AddGroup(const group&	g);
	virtual int QueryGroupCollection(vector<group>& v);
	virtual int UpdateFriend(const friendsinfo& f);
	virtual int DeleteFriend(const friendsinfo& f);
	// ʵ�ʲ���������groupid Ϊ�գ���Ψһ��ʾ�Ǹ���groupname
	virtual int QueryFriendsCollection(vector<friendsinfo>& v, const string & groupid="", const string& groupname="");
	virtual int QueryMsg(msginfo& msg, const string & id, int msgid);
	virtual int QueryMsgCollection(vector<msginfo>& v, const string&  id, int msgid, bool bLessComparison=true,  bool bAsc=false , int num=50,int offset=0   );
	virtual int QueryMsgCollection(vector<msginfo>& v, const string& id, const string& formDate,  const string & toDate );
	//virtual int QueryMsgCollection(vector<msginfo>& v, const string & id)
// 	virtual int AddTable(const string& id);
// 	virtual int DropTable(const string& id);
	virtual int AddMsg(const msginfo& msg, const string & id="");
	virtual int DeleteMsg(int msgid, const string & id);
	virtual int DeleteMsgCollection(int msgidfrom, int nCounts , const string & id );
	virtual bool IsExist(const string& id, e_msginfo_field field, e_comparison_operators comparison, const string& value, int num = 1);
	virtual void FinalRelease();
private:
	int QueryMsgCollection(vector<msginfo>&v,  const char* sql);
};
#endif



#ifdef __cplusplus
extern "C"{
#endif

#ifdef DATASTORAGE_EXPORTS

	//int db_open_database(sqlite3** ppdb, DB_FLAG dbtype);
	//int db_create_db(sqlite3* pdb, DB_FLAG dbtype);

#endif
	//DATADLL_API int  Test();
	DATADLL_API IUserDb* CreateUserDb();
	DATADLL_API IMsgDb* CreateMsgDb();
	DATADLL_API IIni*  CreateIni();
	//����ϵͳ�����иı���û���Ŀ¼���õ��ô˽ӿ�
	DATADLL_API void SeDirectory(LPCTSTR  lptStr);
#ifdef __cplusplus
}
#endif

//
#endif