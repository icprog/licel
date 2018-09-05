#include "StdAfx.h"
#include "DatRecord.h"

// double gb_tmpVal[10];
// char gb_tmpArray[1024*1024*2];
// string gb_tmpString;
// int gb_pos,gb_iStart,gb_iEnd;
// int gb_line=0;
// CString gb_strPropertyName,gb_strPropertyVal;
// vector<CString> gb_colStr;

char* CDatRecord::m_RecordTypeString[] = {"Lidar原始数据","Lidar产品数据"};

CDatRecord::CDatRecord()
{
	// m_pSamples = NULL;
	// m_pExtinctionCoefficient = NULL;
	// m_pDeplorRatio = NULL;
	// m_pCloud = NULL;
	// m_pOpticDepth= NULL;
}

CDatRecord::~CDatRecord(void)
{
	Clear();
}

// BOOL CDatRecord::LoadItemSrc(CString file)
// {
// 	ifstream fi(file);
// 	if(!fi.is_open())
// 		return FALSE;
// 	if(!ParserHeader(fi))
// 		return FALSE;
// 	if(!ParserDatas(fi))
// 		return FALSE;

// 	fi.close();
// 	return TRUE;
// }

// BOOL CDatRecord::LoadItemHeader(CString file)
// {
// 	ifstream fi(file);
// 	if(!fi.is_open())
// 		return FALSE;
// 	if(!ParserHeader(fi))
// 		return FALSE;
// 	fi.close();
// 	return TRUE;
// }

// BOOL CDatRecord::LoadItemData(CString file)
// {
// 	ifstream fi(file);
// 	if(!fi.is_open())
// 		return FALSE;
// 	if(!ParserDatas(fi))
// 		return FALSE;
// 	fi.close();
// 	return TRUE;
// }

void CDatRecord::Clear()
{
	// if(m_pSamples != NULL)
	// {
	// 	m_pSamples->Release();
	// 	m_pSamples = NULL;
	// }
	// if(m_pExtinctionCoefficient != NULL)
	// {
	// 	m_pExtinctionCoefficient->Release();
	// 	m_pExtinctionCoefficient = NULL;
	// }
	// if(m_pDeplorRatio != NULL)
	// {
	// 	m_pDeplorRatio->Release();
	// 	m_pDeplorRatio = NULL;
	// }
	// if(m_pCloud != NULL)
	// {
	// 	m_pCloud->Release();
	// 	m_pCloud = NULL;
	// }
	// if(m_pOpticDepth != NULL)
	// {
	// 	m_pOpticDepth->Release();
	// 	m_pOpticDepth = NULL;
	// }
}

BOOL CDatRecord::AttachRecordFile(CString recordFile,fpHeaderDecoder fp)
{
	return TRUE;
}

BOOL CDatRecord::DecodeStationInfo(fpRadarStationDecoder fp)
{
	return TRUE;
}

BOOL CDatRecord::DecodeRecordProperty(fpRecordPropertyDecoder fp)
{
	return TRUE;
}

BOOL CDatRecord::DecodeChannelProperty(fpChannelDecoder fp)
{
	return TRUE;
}

BOOL CDatRecord::DecodeChannelSample(fpChannelDecoder fp)
{
	return TRUE;
}

BOOL StoreToFile(CString folder)
{
	
	return TRUE;
}