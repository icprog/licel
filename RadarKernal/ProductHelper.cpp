#include "StdAfx.h"
#include "ProductHelper.h"
#include "ProgressDlg.h"
#include "AcquireHelper.h"

char* CProductHelper::CHANNEL_NAME[] = {"CH1-光子","CH2-光子","CH3-光子"};
char* CProductHelper::PRODUCT_NAME[] = {"原始信号","散射系数","消光系数","退偏振比","云信息","光学厚度","边界层高度","垂直能见度","污染物混合层高度","颗粒物质量浓度"};
char* CProductHelper::PRODUCT_FORMAT[] = {"%.0f","%.0f","%.2f","%.2f","%.0f","%.2f","%.0f","%.2f","%.2f","%.2f"};
char* CProductHelper::PRODUCT_UNIT[] = {"","","","","m","","m","","m",""};
char* CProductHelper::PRODUCT_NAME_FORMAT[] = {"原始信号:%.0f","散射系数:%.0f","消光系数:%.2f","退偏振比:%.2f","云信息:%.0fm","光学厚度:%.2f","边界层高度:%.0fm","垂直能见度:%.2f","污染物混合层高度:%.2f","颗粒物质量浓度:%.2f"};
char* CProductHelper::PRODUCT_NAME_UNIT[] = {"原始信号","散射系数","消光系数","退偏振比","云信息(m)","光学厚度","边界层高度(m)","垂直能见度","污染物混合层高度(m)","颗粒物质量浓度"};
unsigned int CProductHelper::CHANNEL_WAVE_LENGTH[] = {355,532,532};
CProductHelper::ChannelWaveState CProductHelper::CHANNEL_WAVE_STATE[] = {CProductHelper::ChannelWaveState_N,
															CProductHelper::ChannelWaveState_P,
															CProductHelper::ChannelWaveState_S};

CProductHelper::CProductHelper(void)
{
	m_pModel = new CDatModel();
}

CProductHelper::~CProductHelper(void)
{
}

void CProductHelper::LoadRecords(vector<CDatRecord *> *pRecordList)
{
	/*
	m_pModel->ClearModel();
	m_pModel->m_pRecords = pRecordList;
	std::vector<CDatRecord *>::iterator it = m_pModel->m_pRecords->begin();
	unsigned int idx = 0,sum;
	sum = m_pModel->m_pRecords->size();
	
	CProgressDlg* dlg = new CProgressDlg();
	dlg->Create(IDD_PROGRESSDLG);
	dlg->ShowWindow(SW_SHOW);
	dlg->CenterWindow();

	//BeginWaitCursor();
	while(it != m_pModel->m_pRecords->end())
	{
		(*it)->LoadItemData((*it)->m_FilePath);
		it++;
		idx++;

		dlg->SetProgress(idx * 100 / sum);
	}
	//EndWaitCursor();

	dlg->CloseWindow();*/
}

CDatRecord* CProductHelper::AddEmptyRecord(CAcquireHelper* pAcquireHelper)
{
	CDatRecord* pRecord = m_pModel->AddEmptyRecord(pAcquireHelper->m_ChannelNum,
												   pAcquireHelper->m_CurvePoint,
												   pAcquireHelper->m_pRange);
	pRecord->m_Time = CTime::GetCurrentTime();
	pRecord->m_FyAngle = 90.0;
	pRecord->m_FwAngle = 0.0;
	pRecord->m_SampleFreq = pAcquireHelper->m_SampleFreq;
	pRecord->m_Chs = pAcquireHelper->m_ChannelNum;
	pRecord->m_SampleNum = pAcquireHelper->m_CurvePoint;
	pRecord->m_FramePeriod = pAcquireHelper->m_UpdatePeriod;
	return pRecord;
}

void CProductHelper::Clear()
{
	m_pModel->ClearModel();
	m_pModel->PerpareModel();
}

void CProductHelper::Remove(unsigned int count)
{
	m_pModel->RemoveRecord(count);
}

void CProductHelper::Regular(CDatRecord* pRecord)
{
	if(pRecord->m_pRegular == NULL)
		pRecord->m_pRegular = new CSample(pRecord->m_pSamples->GetCol(),pRecord->m_pSamples->GetLength(),TRUE);

	int chs = pRecord->m_Chs;
	int len = pRecord->m_SampleNum;

	for(int i=0;i<chs;i++)
	{
		RegularCalc(pRecord->m_pSamples->m_pData[i+1],len,pRecord->m_pRegular->m_pData[i+1]);
	}
}

void CProductHelper::Depolar(CDatRecord* pRecord)
{
	if(pRecord->m_pDeplorRatio == NULL)
		pRecord->m_pDeplorRatio = new CSample(2,pRecord->m_pSamples->GetLength(),TRUE);

	DepolarCalc(2.55,
				pRecord->m_pSamples->GetLength(),
				pRecord->m_pRegular->m_pData[2],
				pRecord->m_pRegular->m_pData[3],
				pRecord->m_pDeplorRatio->m_pData[1]);
}

void CProductHelper::BackScatter(CDatRecord* pRecord)
{
	int ch = pRecord->m_pSamples->GetCol() - 1;
	int datanum = pRecord->m_pSamples->GetLength();
	double rangeResolution = pRecord->m_pSamples->m_pData[0][1] - pRecord->m_pSamples->m_pData[0][0];

	if(pRecord->m_pBackScatter == NULL)
		pRecord->m_pBackScatter = new CSample(ch+1,datanum,TRUE);

	double wavelength[3] = {355,532,532};
	for(int i=0;i<ch;i++)
	{
		BackScatterCalc(datanum,
						pRecord->m_pRegular->m_pData[i+1],
						rangeResolution,
						2000,
						wavelength[i],
						pRecord->m_pDeplorRatio->m_pData[1],
						pRecord->m_pBackScatter->m_pData[i+1]);
	}
}

void CProductHelper::Extinction(CDatRecord* pRecord)
{
	int ch = pRecord->m_pSamples->GetCol() - 1;
	int datanum = pRecord->m_pSamples->GetLength();

	if(pRecord->m_pExtinctionCoefficient == NULL)
		pRecord->m_pExtinctionCoefficient = new CSample(ch+1,datanum,TRUE);

	for(int i=0;i<ch;i++)
	{
		ExtinctionCalc(datanum,
						pRecord->m_pBackScatter->m_pData[i+1],
						pRecord->m_pExtinctionCoefficient->m_pData[i+1]);
	}
}

void CProductHelper::Cloud(CDatRecord* pRecord)
{
	if(pRecord->m_pCloud == NULL)
	{
		pRecord->m_pCloud = new CSample(2,pRecord->m_pSamples->GetLength(),TRUE);
		pRecord->m_pCloud->Zero();
	}

	int layer;
	double cloud_base[100];
	double cloud_top[100];
	int base_idx,top_idx,idx;

	CloudCalc(pRecord->m_pSamples->m_pData[0][1] - pRecord->m_pSamples->m_pData[0][0],
				pRecord->m_pRegular->m_pData[3],
				pRecord->m_pSamples->GetLength(),
				cloud_base,
				cloud_top,
				layer);

	idx = 0;
	for(int j=0;j<layer;j++)
	{
		while(idx < pRecord->m_pSamples->GetLength())
		{
			if(pRecord->m_pSamples->m_pData[0][idx] < cloud_base[j])
			{
				idx++;
				continue;
			}

			base_idx = idx;
			break;
		}
		while(idx < pRecord->m_pSamples->GetLength())
		{
			if(pRecord->m_pSamples->m_pData[0][idx] < cloud_top[j])
			{
				idx++;
				continue;
			}

			top_idx = idx;
			break;
		}
		for(int k=base_idx;k<top_idx;k++)
			pRecord->m_pCloud->m_pData[1][k] = 1;
	}
}

void CProductHelper::OpticDepth(CDatRecord* pRecord)
{
	if(pRecord->m_pOpticDepth == NULL)
		pRecord->m_pOpticDepth = new CSample(pRecord->m_pSamples->GetCol(),1,TRUE);

	for(unsigned int i=1;i<pRecord->m_pSamples->GetCol();i++)
		OpticDepthCalc(pRecord->m_pSamples->m_pData[0][1] - pRecord->m_pSamples->m_pData[0][0],pRecord->m_pExtinctionCoefficient->m_pData[i],pRecord->m_pSamples->GetLength(),pRecord->m_pOpticDepth->m_pData[i]);
}

void CProductHelper::Boundary(CDatRecord* pRecord)
{
	int ch = pRecord->m_pSamples->GetCol() - 1;
	int datanum = pRecord->m_pSamples->GetLength();

	if(pRecord->m_pBoundary == NULL)
		pRecord->m_pBoundary = new CSample(ch + 1,1,TRUE);

	for(int i=0;i<ch;i++)
		BoundaryCalc(pRecord->m_pSamples->m_pData[0],
						pRecord->m_pRegular->m_pData+1,
						datanum,
						1,
						pRecord->m_FramePeriod,
						pRecord->m_pBoundary->m_pData[i+1]);
}

void CProductHelper::Visiblity(CDatRecord* pRecord)
{
	int ch = pRecord->m_pSamples->GetCol() - 1;
	int datanum = pRecord->m_pSamples->GetLength();

	if(pRecord->m_pVisiblity == NULL)
		pRecord->m_pVisiblity = new CSample(pRecord->m_pSamples->GetCol(),pRecord->m_pSamples->GetLength(),TRUE);

	for(int i=0;i<ch;i++)
	{
		VisiblityCalc(pRecord->m_pExtinctionCoefficient->m_pData[i+1],
						datanum,
						pRecord->m_pVisiblity->m_pData[i+1]);
	}
}

void CProductHelper::Mix(CDatRecord* pRecord)
{
	int ch = pRecord->m_pSamples->GetCol() - 1;
	int datanum = pRecord->m_pSamples->GetLength();

	if(pRecord->m_pMix == NULL)
		pRecord->m_pMix = new CSample(ch + 1,1,TRUE);

	for(int i=0;i<ch;i++)
		MixCalc(pRecord->m_pSamples->m_pData[0][1] - pRecord->m_pSamples->m_pData[0][0],
					pRecord->m_pExtinctionCoefficient->m_pData[i+1],
					datanum,
					pRecord->m_pBoundary->m_pData[i+1]);
}

void CProductHelper::GasoloidPotency(CDatRecord* pRecord)
{
	int datanum = pRecord->m_pSamples->GetLength();

	if(pRecord->m_pGasoloidPotency == NULL)
		pRecord->m_pGasoloidPotency = new CSample(2,datanum,TRUE);

	GasoloidPotencyCalc(pRecord->m_pExtinctionCoefficient->m_pData[2],
						datanum,
						pRecord->m_pGasoloidPotency->m_pData[1]);
}

CDatRecord* CProductHelper::SearchRecord(CTime &t)
{
	if(m_pModel->m_pRecords == NULL)
		return NULL;

	if(m_pModel->m_pRecords->size() == 0)
		return NULL;
 
	CDatRecord* pRecord = NULL;
	vector<CDatRecord* >::iterator it = m_pModel->m_pRecords->begin();
	unsigned int interval = (*it)->m_FramePeriod;
	CTimeSpan tSpan;
	while(it != m_pModel->m_pRecords->end())
	{
		tSpan = (*it)->m_Time - t;
		if(tSpan.GetTotalSeconds()<interval)
		{
			pRecord = (*it);
			break;
		}

		it++;
	}
	return pRecord;
}
