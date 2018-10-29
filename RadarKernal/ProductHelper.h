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
		ProductType_BackScatter,
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
		ProductChannelType_Sum
	}ProductChannelType;

	typedef enum tagChannelWaveState
	{
		ChannelWaveState_N,
		ChannelWaveState_P,
		ChannelWaveState_S,
		ChannelWaveState_Laman,
		ChannelWaveState_Sum
	}ChannelWaveState;

	static char* PRODUCT_NAME[ProductType_Sum];
	static char* CHANNEL_NAME[ProductChannelType_Sum];
	static char* PRODUCT_FORMAT[ProductType_Sum];
	static char* PRODUCT_UNIT[ProductType_Sum];
	static char* PRODUCT_NAME_FORMAT[ProductType_Sum];
	static char* PRODUCT_NAME_UNIT[ProductType_Sum];
	static char* WAVE_STATE[ChannelWaveState_Sum];

	static unsigned int CHANNEL_WAVE_LENGTH[ProductChannelType_Sum];
	static ChannelWaveState CHANNEL_WAVE_STATE[ProductChannelType_Sum];

	CDatModel *m_pModel;

public:
	void LoadRecords(vector<CDatRecord *> *pRecordList);
	CDatRecord* SearchRecord(CTime &t);
	CDatRecord* AddEmptyRecord(CAcquireHelper* pAcquireHelper);
	void Clear();
	void Remove(unsigned int count);
	unsigned int GetCount(){ return m_pModel->Count(); }


public:
	static void Regular(CDatRecord* pRecord);
	static void Depolar(CDatRecord* pRecord);
	static void BackScatter(CDatRecord* pRecord);
	static void Extinction(CDatRecord* pRecord);
	
	static void Cloud(CDatRecord* pRecord);
	static void OpticDepth(CDatRecord* pRecord);
	static void Boundary(CDatRecord* pRecord);
	static void Visiblity(CDatRecord* pRecord);
	static void Mix(CDatRecord* pRecord);
	static void GasoloidPotency(CDatRecord* pRecord);
};

