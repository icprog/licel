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
	vector<CDatRecord* > *m_pRecords;

public:
	void ClearModel();
	void PerpareModel();
	void AddRealTimeRecord(CDatRecord* pRecord);
	CDatRecord* AddEmptyRecord(unsigned int sampleChannels,unsigned int sampleNums,double *pRange);
	int SetAtomshereModel(CString path);
};

