﻿#pragma once

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

    static char* m_RecordTypeString[RecordType_Sum];

	typedef void(*fpHeaderDecoder)(unsigned char*, CDatRecord*);
	typedef void(*fpRadarStationDecoder)(unsigned char*, CRadarStation*);
	typedef void(*fpRecordPropertyDecoder)(unsigned char*, CRecordProperty*);
	typedef void(*fpChannelDecoder)(unsigned char*, std::vector<CChannel *>&);

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

