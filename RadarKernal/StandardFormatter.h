#pragma once

#include "RadarStation.h"

class CStandardFormatter
{
public:
	CStandardFormatter(void);
	~CStandardFormatter(void);

	typedef enum tagFormatType
	{
		FormatType_Lidar = 0,

	}FormatType;

public:
	void Serial2File(CString file,FormatType type,CDatRecord* pRecord,CRadarStation* pStation);
	void ReserialFromFile(CString file);
};

