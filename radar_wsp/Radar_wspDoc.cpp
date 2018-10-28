// radar_wspDoc.cpp : implementation of the Cradar_wspDoc class
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "RadarFormView.h"
#include "AdvancedProductView.h"
#include "Radar_wspDoc.h"
#include "MainFrm.h"

#include <fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CRadar_wspDoc, CDocument)

BEGIN_MESSAGE_MAP(CRadar_wspDoc, CDocument)
END_MESSAGE_MAP()

CRadar_wspDoc::CRadar_wspDoc()
{
	CString appPath = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();

	// 配置曲线显示参数
	CString axisConfigFile = appPath + _T("configs\\axis.ini");
	m_AxisConfigHelper.ConfigFromFile(axisConfigFile);
	
	// 配置licel卡参数
	m_AcquireHelper.ConfigLicelCard(appPath + _T("configs\\licel.ini"));
	
	// 配置残疾参数
	CString settingsFile = appPath + _T("configs\\capture.ini");
	m_AcquireHelper.SetDtScalerFactor(GetPrivateProfileInt(_T("fixed"),_T("period"),0,settingsFile));

	// 配置保存路径参数
	LoadSavePath();

	m_WorkMode = WorkMode_FILE;
	m_pCurrRecord = NULL;
	m_curProductPageType = ProductPage_Simple;
}

CRadar_wspDoc::~CRadar_wspDoc()
{
	m_ProductHelper.Clear();
}

BOOL CRadar_wspDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CRadar_wspDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
		
	}
}

#ifdef _DEBUG
void CRadar_wspDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRadar_wspDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif 

BOOL CRadar_wspDoc::OpenRecords(vector<CDatRecord *> *pRecordList)
{
	m_ProductHelper.LoadRecords(pRecordList);
	
	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pSimpleView = (CRadarFormView *)this->GetNextView(ps);
	pSimpleView->UpdateView(m_ProductHelper.m_pModel);

	CAdvancedProductView *pAdvancedView = (CAdvancedProductView *)this->GetNextView(ps);
	pAdvancedView->UpdateView(m_ProductHelper.m_pModel);

	return TRUE;
}

BOOL CRadar_wspDoc::AddProduct(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	if(product >= CProductHelper::ProductType_Cloud)
	{
		POSITION ps = GetFirstViewPosition();
		GetNextView(ps);
		CAdvancedProductView *pAdvancedView = (CAdvancedProductView*)GetNextView(ps);

		if(pAdvancedView->IsProductViewExist(product,ch))
		{
			if(m_curProductPageType == ProductPage_Advanced)
			{
				pAdvancedView->DelProductView(product,ch);
				return TRUE;
			}
			else
			{
				ProductViewSwitch(ProductPage_Advanced);
				return TRUE;
			}
		}
		else if(pAdvancedView->AddProductView(product,ch))
		{
			if(m_curProductPageType != ProductPage_Advanced)
				ProductViewSwitch(ProductPage_Advanced);
			pAdvancedView->UpdateView(m_ProductHelper.m_pModel,product,ch);
			return TRUE;
		}
		else return FALSE;
	}
	else
	{
		POSITION ps = GetFirstViewPosition();
		CRadarFormView* pSimpleView = (CRadarFormView*)GetNextView(ps);

		if(pSimpleView->IsProductViewExist(product,ch))
		{
			if(m_curProductPageType == ProductPage_Simple)
			{
				pSimpleView->DelProductView(product,ch);
				return TRUE;
			}
			else
			{
				ProductViewSwitch(ProductPage_Simple);
				return TRUE;
			}
		}
		else if(pSimpleView->AddProductView(product,ch))
		{
			if(m_curProductPageType != ProductPage_Simple)
				ProductViewSwitch(ProductPage_Simple);
			pSimpleView->UpdateView(m_ProductHelper.m_pModel,product,ch);
			return TRUE;
		}
		else return FALSE;
	}
}

void CRadar_wspDoc::AddRealTimeRecord2Model(CDatRecord *pRecord)
{
	if(m_WorkMode != WorkMode_RT)
		return;

	m_pCurrRecord->m_pSamples->Acc(pRecord->m_pSamples);

	CProductHelper::Regular(m_pCurrRecord);
	CProductHelper::Depolar(m_pCurrRecord);
	CProductHelper::BackScatter(m_pCurrRecord);
	CProductHelper::Extinction(m_pCurrRecord);

	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pView = (CRadarFormView *)this->GetNextView(ps);
	pView->UpdateView(m_pCurrRecord);
}

 void CRadar_wspDoc::SaveDatFile( CDatRecord *pRecord)
 {
 	CTime tCur = CTime::GetCurrentTime();
 	CStringA strTemp;
 	CString strFolder;

 	strFolder.Format(_T("\\%d年"),tCur.GetYear());
 	strFolder = m_SavePath + strFolder;
 	if(!PathFileExists(strFolder))
 		CreateDirectory(strFolder,NULL);
	
 	strFolder.Format(_T("\\%d年\\%02d月"),tCur.GetYear(),tCur.GetMonth());
 	strFolder = m_SavePath + strFolder;
 	if(!PathFileExists(strFolder))
 		CreateDirectory(strFolder,NULL);
	
 	strFolder.Format(_T("\\%d年\\%02d月\\%02d日"),tCur.GetYear(),tCur.GetMonth(),tCur.GetDay());
 	strFolder = m_SavePath + strFolder;
 	if(!PathFileExists(strFolder))
 		CreateDirectory(strFolder,NULL);

 	strFolder = tCur.Format(_T("\\%Y年\\%m月\\%d日\\%H%M%S.rads"));
 	strFolder = m_SavePath + strFolder;
 	//pRecord->Serial2File(strFolder);
 }

void CRadar_wspDoc::StartRealTime(unsigned int period)
{
	m_AcquireHelper.SetDtScalerFactor(period);

	StartRealTime();
}

void CRadar_wspDoc::StartRealTime()
{
	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pSimpleView = (CRadarFormView *)this->GetNextView(ps);
	pSimpleView->StartRealTimeView(&m_AcquireHelper);
	
	CAdvancedProductView *pAdvancedView = (CAdvancedProductView *)this->GetNextView(ps);
	pAdvancedView->StartRealTimeView(&m_AcquireHelper);
	
	m_ProductHelper.Clear();
	m_pCurrRecord = m_ProductHelper.AddEmptyRecord(&m_AcquireHelper);
	m_WorkMode = WorkMode_RT;
}

void CRadar_wspDoc::RealTimeHook()
{
	if(m_WorkMode != WorkMode_RT)
		return;

	CProductHelper::Regular(m_pCurrRecord);

	if(m_pCurrRecord->m_pDeplorRatio == NULL)
			CProductHelper::Depolar(m_pCurrRecord);

	if(m_pCurrRecord->m_pBackScatter == NULL)
		CProductHelper::BackScatter(m_pCurrRecord);

	if(m_pCurrRecord->m_pExtinctionCoefficient == NULL)
		CProductHelper::Extinction(m_pCurrRecord);

	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pSimpleView = (CRadarFormView *)this->GetNextView(ps);
	pSimpleView->UpdateView(m_pCurrRecord,TRUE);

	CProductHelper::Cloud(m_pCurrRecord);
	CProductHelper::OpticDepth(m_pCurrRecord);
	CProductHelper::Boundary(m_pCurrRecord);
	CProductHelper::Visiblity(m_pCurrRecord);
	CProductHelper::Mix(m_pCurrRecord);
	CProductHelper::GasoloidPotency(m_pCurrRecord);

	CAdvancedProductView *pAdvancedView = (CAdvancedProductView *)this->GetNextView(ps);
	pAdvancedView->UpdateView(m_pCurrRecord);

	if(m_bSaveEnable)
		SaveDatFile(m_pCurrRecord);

	m_pCurrRecord = m_ProductHelper.AddEmptyRecord(&m_AcquireHelper);
}

void CRadar_wspDoc::StopRealTime()
{
	m_WorkMode = WorkMode_FILE;
	m_ProductHelper.Clear();
	m_pCurrRecord = NULL;
}

void CRadar_wspDoc::LoadSavePath()
{
	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\settings.ini");

	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("save"),_T("savepath"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	m_SavePath.Empty();
	m_SavePath.Append((LPWSTR)cBuf);

	if(!PathFileExists(m_SavePath))
	{
		MessageBox(NULL,_T("存储路劲:")+m_SavePath+_T("不存在，请重新设置！"),_T("错误"),MB_ICONERROR);
		m_bSaveEnable = FALSE;
	}
	else
	{
		m_bSaveEnable = TRUE;
	}
}

void CRadar_wspDoc::ProductViewSwitch(ProductPageType type)
{
	if(m_curProductPageType == type)
		return;

	m_curProductPageType = type;

	POSITION ps = GetFirstViewPosition();
	CView* pView;
	while(ps != NULL)
	{
		pView = GetNextView(ps);
		pView->ShowWindow(FALSE);

	}

	ps = GetFirstViewPosition();
	int i=0;
	while(i<type)
	{
		GetNextView(ps);
		i++;
	}
	pView = GetNextView(ps);

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->SetActiveView(pView,0);
	pView->ShowWindow(SW_SHOW);
	
	pView->Invalidate(true);
}

void CRadar_wspDoc::ProductViewSwitch()
{
	ProductPageType pageType;
	if(m_curProductPageType == ProductPage_Simple)
		pageType = ProductPage_Advanced;
	else pageType = ProductPage_Simple;
	ProductViewSwitch(pageType);
}
