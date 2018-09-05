#include "StdAfx.h"
#include "DatModel.h"

CDatModel::CDatModel(void)
{
	m_pLidarRecords = NULL;
}

CDatModel::~CDatModel(void)
{
}

void CDatModel::AddRealTimeLidarRecord(CDatRecord* pLidarRecord)
{
	m_pLidarRecords->push_back(pLidarRecord);
}

CDatRecord* CDatModel::AddEmptyLidarRecord(unsigned int ch_count,unsigned int sample_count)
{
	CDatRecord* pLidarRecord = new CDatRecord();
	for(int i=0;i<ch_count;i++)
	{
		CChannel* pChannel = new CChannel();
		pChannel->m_pChannelProperty = new CChannelProperty();
		pChannel->m_pChannelProperty->m_distCount = sample_count;
		pChannel->m_pSample = new CSample(1,sample_count,TRUE);
		pLidarRecord->m_channels.push_back(pChannel);
	}
	m_pLidarRecords->push_back(pLidarRecord);
	return pLidarRecord;
}

int CDatModel::SetAtomshereModel(CString path)
{
	int item = LoadAtmosphereModel(((CStringA)path).GetBuffer());
	return item;
}

void CDatModel::ClearModel()
{
	if(pLidarRecord == NULL)
		return;

	vector<CDatRecord *>::iterator it = m_pLidarRecords->begin();
	while(it != m_pLidarRecords->end())
	{
		vector<CChannle *>::iterator iit = (*it)->m_channels.begin();
		while(iit != (*it)->m_channels.end())
		{
			(*iit)->m_pSample->Release();

			delete (CChannel *)(*iit);
			iit++;
		}

		delete (CDatRecord)(*it);
		it++;
	}

	delete m_pLidarRecords;
	m_pLidarRecords = NULL;
}

void CDatModel::PerpareModel()
{
	m_pLidarRecords = new vector<CDatRecord *>();
}


