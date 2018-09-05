#include "StdAfx.h"
#include "ProductHelper.h"
#include "ProgressDlg.h"
#include "AcquireHelper.h"

char* CProductHelper::CHANNEL_NAME[] = {"CH1-光子","CH1-模拟","CH2-光子","CH2-模拟","CH3-光子","CH3-模拟"};
char* CProductHelper::PRODUCT_NAME[] = {"原始信号","消光系数","退偏振比","云信息","光学厚度","边界层高度","垂直能见度","污染物混合层高度","颗粒物质量浓度"};
char* CProductHelper::PRODUCT_FORMAT[] = {"%.0f","%.2f","%.2f","%.0f","%.2f","%.0f","%.2f","%.2f","%.2f"};
char* CProductHelper::PRODUCT_UNIT[] = {"","","","m","","m","","m",""};
char* CProductHelper::PRODUCT_NAME_FORMAT[] = {"原始信号:%.0f","消光系数:%.2f","退偏振比:%.2f","云信息:%.0fm","光学厚度:%.2f","边界层高度:%.0fm","垂直能见度:%.2f","污染物混合层高度:%.2f","颗粒物质量浓度:%.2f"};
char* CProductHelper::PRODUCT_NAME_UNIT[] = {"原始信号","消光系数","退偏振比","云信息(m)","光学厚度","边界层高度(m)","垂直能见度","污染物混合层高度(m)","颗粒物质量浓度"};

CProductHelper::CProductHelper(void)
{
	m_pModel = new CDatModel();
}

CProductHelper::~CProductHelper(void)
{
}

BOOL CProductHelper::LoadAtmosphereModel(CString modelPath)
{
	m_pModel->SetAtomshereModel(modelPath);
	return TRUE;
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
	CDatRecord* pRecord = m_pModel->AddEmptyLidarRecord(pAcquireHelper->m_ChannelNum,
												   pAcquireHelper->m_CurvePoint);
	// 设置站点属性
	pRecord->m_pRadarStation = &(pAcquireHelper->m_RadarStation);

	// 设置记录属性
	pRecord->m_pRecordProperty = &(pAcquireHelper->m_RecordProperty);

	// 设置通道属性
	std::vector<CChannel *>::iterator it = pRecord->m_channels.begin();
	unsigned int idx = 0;
	while(it != pRecord->m_channels.end())
	{
		(*it)->m_pChannelProperty = pAcquireHelper->m_ChannelPropertys[idx];
		it++;
		idx++;
	}

	return pRecord;
}

void CProductHelper::Clear()
{
	m_pModel->ClearModel();
	m_pModel->PerpareModel();
}

// void CProductHelper::Extinction(CDatRecord* pRecord)
// {
// 	if(pRecord->m_pExtinctionCoefficient == NULL)
// 		pRecord->m_pExtinctionCoefficient = new CSample(pRecord->m_pSamples->GetCol(),pRecord->m_pSamples->GetLength(),TRUE);

// 	for(unsigned int i=1;i<pRecord->m_pSamples->GetCol();i++)
// 		Fernald(pRecord->m_pSamples->m_pData[0],pRecord->m_pSamples->m_pData[i],pRecord->m_pSamples->GetLength(),pRecord->m_pExtinctionCoefficient->m_pData[i]);
// }

// void CProductHelper::Depolar(CDatRecord* pRecord)
// {
// 	if(pRecord->m_pDeplorRatio == NULL)
// 		pRecord->m_pDeplorRatio = new CSample(2,pRecord->m_pSamples->GetLength(),TRUE);

// 	DepolarCalc(2.55,
// 				pRecord->m_pSamples->GetLength(),
// 				pRecord->m_pSamples->m_pData[2],
// 				pRecord->m_pSamples->m_pData[3],
// 				pRecord->m_pDeplorRatio->m_pData[1]);
// }

// void CProductHelper::Cloud(CDatRecord* pRecord)
// {
// 	if(pRecord->m_pCloud == NULL)
// 	{
// 		pRecord->m_pCloud = new CSample(pRecord->m_pSamples->GetCol(),pRecord->m_pSamples->GetLength(),TRUE);
// 		for(int i=1;i<pRecord->m_pSamples->GetCol();i++)
// 			memset(pRecord->m_pCloud->m_pData[i],0,pRecord->m_pSamples->GetLength() * sizeof(double));
// 	}

// 	int layer;
// 	double cloud_base[40];
// 	double cloud_top[40];
// 	int base_idx,top_idx,idx;
// 	for(unsigned int i=1;i<pRecord->m_pSamples->GetCol();i++)
// 	{
// 		//云层数存放于对应通道数组的第0个元素，云底存放于从第1个元素起始处，云高存放于从第101个元素起始处
// 		CloudCalc(pRecord->m_pSamples->m_pData[0],
// 					pRecord->m_pSamples->m_pData[i],
// 					pRecord->m_pSamples->GetLength(),
// 					cloud_base,
// 					cloud_top,
// 					layer);
// 		idx = 0;
// 		for(int j=0;j<layer;j++)
// 		{
// 			while(idx < pRecord->m_pSamples->GetLength())
// 			{
// 				if(pRecord->m_pSamples->m_pData[0][idx] < cloud_base[j])
// 					idx++;

// 				base_idx = idx;
// 				break;
// 			}
// 			while(idx < pRecord->m_pSamples->GetLength())
// 			{
// 				if(pRecord->m_pSamples->m_pData[0][idx] < cloud_top[j])
// 					idx++;

// 				top_idx = idx;
// 				break;
// 			}
// 			for(int k=base_idx;k<top_idx;k++)
// 				pRecord->m_pCloud->m_pData[i][k] = 1;
// 		}
// 	}
// }

// void CProductHelper::OpticDepth(CDatRecord* pRecord)
// {
// 	if(pRecord->m_pOpticDepth == NULL)
// 		pRecord->m_pOpticDepth = new CSample(pRecord->m_pSamples->GetCol(),pRecord->m_pSamples->GetLength(),TRUE);

// 	if(pRecord->m_pExtinctionCoefficient == NULL)
// 		Extinction(pRecord);

// 	for(unsigned int i=1;i<pRecord->m_pSamples->GetCol();i++)
// 		OpticDepthCalc(pRecord->m_pSamples->m_pData[0],pRecord->m_pExtinctionCoefficient->m_pData[i],pRecord->m_pSamples->GetLength(),pRecord->m_pOpticDepth->m_pData[i]);
// }
