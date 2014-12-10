// Header file for downloader.


#ifndef DOWNLOAD_H
#define DOWNLOAD_H

//#include <iostream>

const int MAX_FILENAME_SIZE = 512;

#ifdef HTTTPDOWNLOAD_EXPORTS

//#define  HTTPDOWNLOAD_API __declspec(dllexport)  

#include <string>
#include <windows.h>
#include <wininet.h>
#include <fstream>

using namespace std;

const int MAX_ERRMSG_SIZE = 80;

const int BUF_SIZE = 10240;             // 10 KB

#pragma comment(lib, "wininet.lib")
// �Զ����쳣��
class DLExc
{
private:
	char err[MAX_ERRMSG_SIZE];
public:
	DLExc(char *exc)
	{
		if(strlen(exc) < MAX_ERRMSG_SIZE)
			strcpy(err, exc);
	}

	// ����ָ�������Ϣ�ַ�����ָ��
	const char *geterr()
	{
		return err;
	}
};


// httpЭ���ļ�������
class Download
{
private:
	static bool ishttp(const char *url);
	static bool httpverOK(HINTERNET hIurl);
	static bool getfname(const char *url, char *fname);
	static unsigned long openfile(const char *filename, bool reload, ofstream &fout);
public:
	static bool download(const char *url, const char*  filedir, const char* filename , bool reload=false, void (*update)(unsigned long, unsigned long)=NULL);
	static bool download(const char *url, char* pbuf, int bufsize);
};
#else 

//#define  HTTPDOWNLOAD_API  __declspec(dllimport) 

#endif


typedef void  (*download_callback) (unsigned long , unsigned long);

#ifdef __cplusplus

extern "C"  {

#endif	

/*
	fileDir Ϊ�ļ���ŵ�Ŀ¼�� �ò���һ��Ϊ�գ� ��filenameΪ���ǿ�������fileDirĿ¼��
	���ļ���Ĭ�����ƴ���filedirĿ¼��

	filename Ϊ�ļ������ƣ� ����ò���Ϊ�գ� ��ôʹ���ļ�ԭ�е�����

	reload �Ƿ��������� FALSE�Ļ� ����֮ǰ�Ľ����Ͻ������� 

	callbk  ����ָ�룬 ���ûص��������Լ�ص�ǰ�����ؽ���
*/
bool http_downloadfile(const char* url,const char*  fileDir, const char* filename,  bool reload, download_callback callbk);

/*
	������ص�����ͨ�������ļ���txt�� �����ģ� ֻ��Ҫ
*/
bool http_downloadtobuf(const char* url, char* pbuf, int bufSize);

#ifdef __cplusplus

}
#endif

#endif

