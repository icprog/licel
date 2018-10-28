#include "StdAfx.h"
#include "DatModel.h"

CDatModel::CDatModel(void)
{
	m_pRecords = NULL;
}

CDatModel::~CDatModel(void)
{
}

void CDatModel::AddRealTimeRecord(CDatRecord* pRecord)
{
	m_pRecords->push_back(pRecord);
}

CDatRecord* CDatModel::AddEmptyRecord(unsigned int sampleChannels,unsigned int sampleNums,double *pRange)
{
	CDatRecord* pRecord = new CDatRecord();
	pRecord->m_pSamples = new CSample(sampleChannels+1,sampleNums,TRUE);
	pRecord->m_pSamples->Zero();
	pRecord->m_pRegular = new CSample(sampleChannels+1,sampleNums,TRUE);
	pRecord->m_pRegular->Zero();

	memcpy(pRecord->m_pSamples->m_pData[0],pRange,sizeof(double)*sampleNums);
	m_pRecords->push_back(pRecord);
	return pRecord;
}

void CDatModel::ClearModel()
{
	if(m_pRecords == NULL)
		return;

	vector<CDatRecord *>::iterator it = m_pRecords->begin();
	while(it != m_pRecords->end())
	{
		(*it)->Clear();
		it++;
	}
	m_pRecords->clear();	

	delete m_pRecords;
	m_pRecords = NULL;
}

void CDatModel::RemoveRecord(unsigned int count)
{
	if(m_pRecords == NULL)
		return;

	count = min(m_pRecords->size(),count);
	vector<CDatRecord *>::iterator it;
	while(count > 0)
	{
		m_pRecords->at(0)->Clear();
		it = m_pRecords->begin();
		m_pRecords->erase(it);

		count--;
	}
}

unsigned int CDatModel::Count() 
{ 
	if(m_pRecords == NULL)
		return 0;

	return m_pRecords->size(); 
}

void CDatModel::PerpareModel()
{
	m_pRecords = new vector<CDatRecord *>;
}


