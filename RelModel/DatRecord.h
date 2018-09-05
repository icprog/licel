#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "RadarStation.h"
#include "ChannelProperty.h"
#include "Channel.h"
#include "RecordProperty.h"

class __declspec(dllexport) CDatRecord
{
public:
	CDatRecord();
	~CDatRecord(void);

	typedef enum tagRecordType
	{
		RecordType_Lidar = 0, //Lidar原始数据
        RecordType_Product,    //Lidar产品数据
		RecordType_Sum
	}RecordType;

    static char* m_FormatTypeString[FormatType_Sum];

	typedef (void)*(unsigned char*, CDatRecord*) fpHeaderDecoder;
	typedef (void)*(unsigned char*, CRadarStation*) fpRadarStationDecoder;
	typedef (void)*(unsigned char*, CRecordProperty*) fpRecordPropertyDecoder;
	typedef (void)*(unsigned char*, std::vector<CChannle *>&) fpChannelDecoder;

public:
	CString m_src;
	RecordType m_Type; 

	CRadarStation* m_pRadarStation;
	CRecordProperty* m_pRecordProperty;
	std::vector<CChannel *> m_channels;

public:
	void Clear();
	
	BOOL AttachRecordFile(CString recordFile,fpHeaderDecoder fp);
	BOOL DecodeStationInfo(fpRadarStationDecoder fp);
	BOOL DecodeRecordProperty(fpRecordPropertyDecoder fp);
	BOOL DecodeChannelProperty(fpChannelDecoder fp);
	BOOL DecodeChannelSample(fpChannelDecoder fp);

	BOOL StoreToFile(CString folder);
};

