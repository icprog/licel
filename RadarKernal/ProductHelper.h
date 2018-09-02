#pragma once
#include <vector>
using namespace std;

class CAcquireHelper;

class AFX_EXT_CLASS CProductHelper
{
public:
	CProductHelper(void);
	~CProductHelper(void);

	typedef enum tagProductType
	{
		ProductType_OrginSignal = 0,
		ProductType_ExtinctionCoefficient,
		ProductType_DepolarizationRatio,
		ProductType_Cloud,
		ProductType_OpticDepth,
		ProductType_AtmosphereBoundary,
		ProductType_Visiblity,
		ProductType_MixHeight,
		ProductType_MassConcentration,
		ProductType_Sum
	}ProductType;

	typedef enum tagProductChannelType
	{
		ProductChannelType_1ch = 0,
		ProductChannelType_2ch,
		ProductChannelType_3ch,
		ProductChannelType_4ch,
		ProductChannelType_5ch,
		ProductChannelType_6ch,
		ProductChannelType_Sum
	}ProductChannelType;

	static char* PRODUCT_NAME[ProductType_Sum];
	static char* CHANNEL_NAME[ProductChannelType_Sum];
	static char* PRODUCT_FORMAT[ProductType_Sum];
	static char* PRODUCT_UNIT[ProductType_Sum];
	static char* PRODUCT_NAME_FORMAT[ProductType_Sum];
	static char* PRODUCT_NAME_UNIT[ProductType_Sum];

	CDatModel *m_pModel;

public:
	BOOL LoadAtmosphereModel(CString modelPath);
	void LoadRecords(vector<CDatRecord *> *pRecordList);
	CDatRecord* AddEmptyRecord(CAcquireHelper* pAcquireHelper);
	void Clear();

	static void Extinction(CDatRecord* pRecord);
	static void Depolar(CDatRecord* pRecord);
	static void Cloud(CDatRecord* pRecord);
	static void OpticDepth(CDatRecord* pRecord);
};

