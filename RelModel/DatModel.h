#pragma once
#include "DatRecord.h"
#include <vector>
using namespace std;

class __declspec(dllexport) CDatModel
{
public:
	CDatModel(void);
	~CDatModel(void);

public:
	vector<CDatRecord* > *m_pLidarRecords;
	
public:
	void ClearModel();
	void PerpareModel();
	void AddRealTimeLidarRecord(CDatRecord* pRecord);
	CDatRecord* AddEmptyLidarRecord(unsigned int ch_count,unsigned int sample_count);

	int SetAtomshereModel(CString path);
};

