#pragma once

class CStandardFormatter
{
public:
	CStandardFormatter(void);
	~CStandardFormatter(void);

public:
	// 标准文件尺寸
	const static unsigned int StandardFormat_Size = 512310;
	// 标准文件格式版本
	const static unsigned int StandardFormat_Version = 1;

public:
	// 持久化
	static void SerialHeader(unsigned char* pBuf,CDatRecord* pRecord);
	static void SerialStationInfo(unsigned char* pBuf,CRadarStation* pStation);
	static void SerialRecordInfo(unsigned char* pBuf,CRecordProperty* pRecordProperty);
	static void SerialChannelInfo(unsigned char* pBuf,std::vector<CChannel *>& channels);
	static void SerialChannelSample(unsigned char* pBuf,std::vector<CChannel *>& channels);

	// 反持久化
	static int ReserialHeader(unsigned char* pBuf,CDatRecord* pRecord);
	static int ReserialStationInfo(unsigned char* pBuf,CRadarStation* pStation);
	static int ReserialRecordInfo(unsigned char* pBuf,CRecordProperty* pRecordProperty);
	static int ReserialChannelInfo(unsigned char* pBuf,unsigned short chCount,std::vector<CChannel *>& channels);
	static int ReserialChannelSample(unsigned char* pBuf,std::vector<CChannel *>& channels);
};

