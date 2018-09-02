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
	CString settingsFile = appPath + _T("configs\\settings.ini");
	m_AcquireHelper.SetDtScalerFactor(GetPrivateProfileInt(_T("capture"),_T("period"),0,settingsFile));
	LoadSavePath(appPath);
	CString modelFile = appPath + _T("model\\atomspheremodel.mdl");
	m_ProductHelper.LoadAtmosphereModel(modelFile);
	m_pCurrRecord = NULL;
	m_curProductPageType = ProductPage_Simple;
	m_bRTAcquire = FALSE;
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

BOOL CRadar_wspDoc::AddProduct(CProductHelper::ProductType product,unsigned int ch)
{
	if(product >= CProductHelper::ProductType_Cloud)
	{
		ProductViewSwitch(ProductPage_Advanced);

		POSITION ps = GetFirstViewPosition();
		GetNextView(ps);
		CAdvancedProductView *pAdvancedView = (CAdvancedProductView*)GetNextView(ps);

		if(pAdvancedView->IsProductViewExist(product,ch))
		{
			pAdvancedView->DelProductView(product,ch);
			return TRUE;
		}
		if(!pAdvancedView->AddProduct(product,ch))
			return FALSE;

		//FIXME:添加产品至高级产品视图后需刷新视图界面
		//pAdvancedView->UpdateView(m_ProductHelper.m_pModel,product,ch);
	}
	else
	{
		ProductViewSwitch(ProductPage_Simple);

		POSITION ps = GetFirstViewPosition();
		CRadarFormView* pSimpleView = (CRadarFormView*)GetNextView(ps);

		if(pSimpleView->IsProductViewExist(product,ch))
		{
			pSimpleView->DelProductView(product,ch);
			return TRUE;
		}

		if(!pSimpleView->AddProductView(product,ch))
			return FALSE;
		pSimpleView->UpdateView(m_ProductHelper.m_pModel,product,ch);
	}
	return TRUE;
}

void CRadar_wspDoc::AddRealTimeRecord2Model(CDatRecord *pRecord)
{
	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pView = (CRadarFormView *)this->GetNextView(ps);
	m_pCurrRecord->m_pSamples->Acc(pRecord->m_pSamples);

	CProductHelper::Depolar(m_pCurrRecord);
	CProductHelper::Extinction(m_pCurrRecord);
	
	pView->UpdateView(m_pCurrRecord);

	//FIXME:基础产品视图及高级产品视图均需要刷新
	
}

void CRadar_wspDoc::SaveToDir(CDatRecord *pRecord)
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
	m_pCurrRecord->Serial2File(strFolder);
}

void CRadar_wspDoc::StartRealTime(unsigned int period)
{
	m_AcquireHelper.SetDtScalerFactor(period);
	m_AcquireHelper.UpdateHeightScaleFactor();

	StartRealTime();
}

void CRadar_wspDoc::StartRealTime()
{
	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pView = (CRadarFormView *)this->GetNextView(ps);
	pView->StartRealTimeView(&m_AcquireHelper);
	
	//FIXME:基础产品视图及高级产品视图均需更新

	m_ProductHelper.Clear();
	m_pCurrRecord = m_ProductHelper.AddEmptyRecord(&m_AcquireHelper);
	m_bRTAcquire = TRUE;
}

void CRadar_wspDoc::RealTimeHook()
{
	POSITION ps = this->GetFirstViewPosition();
	CRadarFormView *pSimpleView = (CRadarFormView *)this->GetNextView(ps);
	pSimpleView->UpdateView(m_pCurrRecord,TRUE);

	if(m_bSaveEnable)
		SaveToDir(m_pCurrRecord);

	m_pCurrRecord = m_ProductHelper.AddEmptyRecord(&m_AcquireHelper);
}

void CRadar_wspDoc::StopRealTime()
{
	m_ProductHelper.Clear();
	m_pCurrRecord->Clear();
	delete m_pCurrRecord;

	m_bRTAcquire = FALSE;
}

void CRadar_wspDoc::LoadSavePath(CString appPath)
{
	CString iniFile = appPath + _T("configs\\settings.ini");

	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("save"),_T("savepath"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	m_SavePath.Empty();
	m_SavePath.Append((LPWSTR)cBuf);

	if(!PathFileExists(m_SavePath))
	{
		MessageBox(NULL,_T("采集数据文件保存位置")+m_SavePath+_T("不存在，请重新配置采集数据保存位置！"),_T("错误"),MB_ICONERROR);
		m_bSaveEnable = FALSE;
	}
	else
	{
		m_bSaveEnable = TRUE;
	}
}

void CRadar_wspDoc::LoadSavePath()
{
	CString appPath = ((Cradar_wspApp *)AfxGetApp())->GetAppPath();
	LoadSavePath(appPath);
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