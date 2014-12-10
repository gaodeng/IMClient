#pragma once
#include "Path.h"

#include "Utils.h"

#include <vector>

#include "XmlDocument.h"

class CFaceInfo
{
public:
	CFaceInfo(void);
	~CFaceInfo(void);

public:
	int m_nId; //xml��ID
	int m_nIndex; //gif��ǰ׺
	tString m_strTip;
	tString m_strFileName;
};

// class CFaceInfoKey
// {
// public:
// 	CFaceInfoKey();
// 	~CFaceInfoKey();
// 
// 	int  m_nId;   // Ĭ�ϰ���m_nId����
// 	tString m_tstr; // ��������
// 
// 	
// };


//�����ǰ�xml��˳��˳�����е�

class CFaceList
{
public:
	CFaceList(void);
	~CFaceList(void);

public:
	void Reset();
	BOOL LoadConfigFile(LPCTSTR lpszFileName);
	CFaceInfo * GetFaceInfo(int nIndex);   //���ݱ����б�������
	CFaceInfo * GetFaceInfoById(int nFaceId); //���ݱ���xml��id����
	CFaceInfo * GetFaceInfoByFaceTip(LPCTSTR lptStr);
	CFaceInfo * GetFaceInfoByIndex(int nFaceIndex);

public:
	int m_nItemWidth, m_nItemHeight;
	int m_nZoomWidth, m_nZoomHeight;
	int m_nRow, m_nCol;
	//ע������vector �� map����Ķ���ͬһ��ָ���ͷŵ��ͷ�ʱ��һ�ξ���
	std::vector<CFaceInfo *> m_arrFaceInfo;
	//std::map<int, CFaceInfo*> m_mapFaceinfo;
	
};