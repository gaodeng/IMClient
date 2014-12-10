// A file download subsystem


#include "stdafx.h"
#include "Download.h"

/**
����һ���ļ�

���ļ���URL�����ӿں���

filenameΪ�ļ����ص����صĴ洢·��

���ĸ�����Ϊ�ص�����ָ�룬��¼��ǰ���ص�״���� Ĭ��Ϊ��
*/
bool Download::download(const char *url,const char*  fileDir, const char* filename, bool reload, void (*update)(unsigned long, unsigned long))
{
    ofstream fout;              // output stream
    unsigned char buf[BUF_SIZE];// д�� buffer
    unsigned long numrcved;     // ÿ�ζ�ȡ�ĵ��ֽڳ���
    unsigned long filelen;      // �ļ���Ӳ���ϵĳ���
    HINTERNET hIurl = NULL, hInet = NULL;     // internet handles
    unsigned long contentlen;   // length of content
    unsigned long len;          // length of contentlen
    unsigned long total = 0;    // running total of bytes received
    char header[80];            // holds Range header
	char filePath[MAX_FILENAME_SIZE] = {0x0};
    try
    {
        if(!ishttp(url))
			return false;
          //  throw DLExc("Must be HTTP url");

        /*
        fileDirΪ�ļ���Ŀ¼����url�л�ȡ�ļ�����õ��ļ��洢·��
        fout���ش򿪵��ļ���. ���reloadΪ��, ��ô�����ļ���ɾ����
		�����ļ��ĳ��ȣ�����ļ�û���ضϣ��������ء�
        */
		char fname[MAX_PATH] = {0x0};
		if (filename == NULL || strlen(filename) == 0)
		{
			if(!getfname(url, fname))
				return false;
				//throw DLExc("File name error");
		}
		else
		{
			strcpy(fname, filename);
		}
		
		

		if (fileDir != NULL && strlen(fileDir)!=0 )
		{
			sprintf(filePath, "%s\\%s", fileDir, fname);
		}
		else
		{
			sprintf(filePath, "%s",  fname);
		}
		
		//sprintf(szFilePath, "%s\\")
		
       // filelen = openfile(filePath, reload, fout);

        //���������Ƿ����
        if(InternetAttemptConnect(0) != ERROR_SUCCESS)
			return false;
            //throw DLExc("Can't connect");

        // ����������
        hInet = InternetOpen("downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if(hInet == NULL)
            throw DLExc("Can't open connection");

		 filelen = openfile(filePath, reload, fout);

        // ���ݷ�Χ ͷ����
		//Range:bytes=500-  ��ʾ500�ֽ��Ժ�ķ�Χ
		//ʹ�����������Լ���ǰһ�ε�����

        sprintf(header, "Range:bytes=%d-", filelen);

        // ��url
        //hIurl = InternetOpenUrl(hInet, url, header, -1, INTERNET_FLAG_NO_CACHE_WRITE, 0);
        hIurl = InternetOpenUrl(hInet, url, header, strlen(header), INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if(hIurl == NULL)
            throw DLExc("Can't open url");

        // ��֤֧�� HTTP/1.1 ���߸��߰汾
        if(!httpverOK(hIurl))
            throw DLExc("HTTP/1.1 not supported");

        //�õ���Դ����
        len = sizeof contentlen;
		//char szzz[2048] = {0x0};
		if (!HttpQueryInfo(hIurl, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &contentlen, &len, NULL))
		{
			throw DLExc("Can't get status code");
		}

		if (contentlen == 404)
		{
			throw DLExc("404");

		}

        if(!HttpQueryInfo(hIurl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &contentlen, &len, NULL))
            throw DLExc("File or content length not found");
		
        // ��������ص��ļ����ڣ� ����û�����꣬ ��ô�������
        if(filelen != contentlen && contentlen)
        {
            do
            {
                // ��ȡ�ֽ�
                if(!InternetReadFile(hIurl, &buf, BUF_SIZE, &numrcved))
                    throw DLExc("Error occurred during download");

                // д�ֽ�
                fout.write((const char *) buf, numrcved);
                if(!fout.good())
                    throw DLExc("Error writing file");

                // �����ܽ���
                total += numrcved;

                // �ص�������������״̬
                if(update && numrcved > 0)
                    update(contentlen + filelen, total + filelen);
            } while (numrcved > 0);
        }
        else
        {
            if(update)
                update(filelen, filelen);
        }
    }
    catch (DLExc)
    {
        fout.close();
		DeleteFile(filePath);
        InternetCloseHandle(hIurl);
        InternetCloseHandle(hInet);
		//return false;
        // �����ܳ��쳣 ��������ʹ��
        throw;
    }

    fout.close();
    InternetCloseHandle(hIurl);
    InternetCloseHandle(hInet);

    return true;
}

bool Download::download(const char *url, char* pbuf, int bufsize)
{
	//DWORD db ;
	try
	{
		if(!ishttp(url))
			throw DLExc("Must be HTTP url");

		//���������Ƿ����
		if(InternetAttemptConnect(0) != ERROR_SUCCESS)
			throw DLExc("Can't connect");


		// ����������
		 HINTERNET hInet = InternetOpen("downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
		 if(hInet == NULL)
			 throw DLExc("Can't open connection");

		 DWORD options = INTERNET_FLAG_NEED_FILE|INTERNET_FLAG_HYPERLINK|INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_RELOAD;
		HINTERNET  hIurl = InternetOpenUrl(hInet, url, NULL, -1, /*INTERNET_FLAG_NO_CACHE_WRITE*/options, 0);
		if(hIurl == NULL)
			throw DLExc("Can't open url");
		// ��֤֧�� HTTP/1.1 ���߸��߰汾
		if(!httpverOK(hIurl))
			throw DLExc("HTTP/1.1 not supported");

		//�õ���Դ����
 		DWORD content;
 		DWORD len = sizeof content;
// 		if(!HttpQueryInfo(hIurl, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &contentlen, &len, NULL))
// 			throw DLExc("File or content length not found");

		if (!HttpQueryInfo(hIurl, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &content, &len, NULL))
		{
			throw DLExc("Can't get status code");
		}

		if (content >=400)
		{
			char szError[24] = {0x0};
			itoa(content,szError,  10);
			throw DLExc(szError);

		}

		// ��ȡ�ֽ�, �˴��Ƚϼ򵥶��ڱȽϴ���ļ�����Ƕ����ڴ��п��Էֶ�ζ�ȡ
		DWORD dwRedSizeOut;
		if(!InternetReadFile(hIurl, pbuf, bufsize, &dwRedSizeOut))
			{
				throw DLExc("Error occurred during download");
			}
		else
		{
			pbuf[dwRedSizeOut]= 0x0;
			return true;
		}
	}
	catch(DLExc )
	{
		throw;
	}
	return false;
}

// ����true ���http�汾���ڻ����1.1
bool Download::httpverOK(HINTERNET hIurl)
{
    char str[80];
    unsigned long len = 79;

    if(!HttpQueryInfo(hIurl, HTTP_QUERY_VERSION, &str, &len, NULL))
        return false;


    char *p = strchr(str, '/');
    p++;
    if(*p == '0')
        return false;       // ����ʹ�� HTTP 0.x

    //  str��:HTTP/1.1
    p = strchr(str, '.');
    p++;

    // ת��int 
    int minorVerNum = atoi(p);

    if(minorVerNum > 0)
        return true;

    return false;
}


//��url�еõ�http���������ļ�����
bool Download::getfname(const char *url, char *fname)
{
    // �ҵ����һ�� /
   const  char *p = strrchr(url, '/');

    // �������һ�� / ����ļ���
    if(p && (strlen(p) < MAX_FILENAME_SIZE))
    {
        p++;
        strcpy(fname, p);
        return true;
    }
    else
    {
        return false;
    }
}

/*
	������ļ�����ʼ����������������ļ��ĳ��ȡ����reload  Ϊtrue, ��ô��ɾ���Ѿ����ڣ����������δ�����꣩���ļ�
*/
unsigned long Download::openfile(const char * filename, bool reload, ofstream &fout)
{
#if 0
    char fname[MAX_FILENAME_SIZE];

    if(!getfname(url, fname))
        throw DLExc("File name error");

    if(!reload)
        fout.open(fname, ios::binary | ios::out | ios::app | ios::ate);
    else
        fout.open(fname, ios::binary | ios::out | ios::trunc);

    if(!fout)
        throw DLExc("Can't open output file");
#endif

	if(!reload)
		fout.open(filename, ios::binary | ios::out | ios::app | ios::ate);
	else
		fout.open(filename, ios::binary | ios::out | ios::trunc);

	if(!fout)
		throw DLExc("Can't open output file");

 //���س���
    return fout.tellp();
}

// ȷ����http����
bool Download::ishttp(const char *url)
{
    char str[5] = "";

    // 
    strncpy(str, url, 4);

    // 
    for(char *p = str; *p; p++)
        *p = tolower(*p);

    return !strcmp("http", str);
}


//���ؽӿ�ʵ��
bool  http_downloadfile(const char* url,const char*  fileDir, const char* filename, bool reload, download_callback callbk)
{
	/*
		char url[] = "http://img3.wcnimg.com/M00/44/5B/wKgKZVMXChriLAoGAABBjs_j4Go664-100-100.jpg";

		bool reload = false;

		if(argc==2 && !strcmp(argv[1], "reload"))
		reload = true;

		printf("Beginning download\n");

		try
		{
		if(Download::download(url, reload, showprogress))
		printf("Download Complete\n");
		}
		catch(DLExc exc)
		{
		printf("%s\n", exc.geterr());
		printf("Download interrupted\n");
		}

		system("PAUSE");
		return EXIT_SUCCESS;
	*/
	try
	{
		if (Download::download(url,fileDir,filename, reload,  callbk))
		{
			OutputDebugString("Download Complete \n");
		    return true;
		}
		else
			return false;
	}
	catch (DLExc exc)
	{
		OutputDebugString("\n");
		OutputDebugString(exc.geterr());
		OutputDebugString("Download Interrupted \n");
		return false;
	
	}
	return true;
}

bool http_downloadtobuf(const char* url, char* pbuf, int bufSize)
{
	try
	{
		if (Download::download(url, pbuf, bufSize))
		{
			OutputDebugString("Download Complete \n");
			return true;
		}
		else
			return false;
	}
	catch(DLExc exc)
	{
		OutputDebugString("\n");
		OutputDebugString(exc.geterr());
		OutputDebugString("Download Interrupted \n");
		return false;
	}
}