// ProductBaseView.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "ProductBaseView.h"
#include "Radar_wspDoc.h"

// CProductBaseView

IMPLEMENT_DYNCREATE(CProductBaseView, CFormView)

CProductBaseView::CProductBaseView()
	: CFormView(CProductBaseView::IDD)
{
	m_bInitial = FALSE;
	for(int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
		m_pProductView[i] = NULL;

	m_heightInterval = 50;
	m_datetimeInterval = 30;
	m_curvePointsSize = 300;

	m_pSelectedWaterfall = NULL;
	m_pSelectedScope = NULL;
	m_pHookedRecord = NULL;
}

CProductBaseView::~CProductBaseView()
{
}

void CProductBaseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProductBaseView, CFormView)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CProductBaseView 诊断

#ifdef _DEBUG
void CProductBaseView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProductBaseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CProductBaseView::ClearViewData()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		m_pProductView[i]->m_pWaterfall->Data.Clear();
	}
}

BOOL CProductBaseView::DelProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	if(!IsProductViewExist(product,ch))
		return TRUE;

	int idx = GetProductViewIdx(product,ch);
	delete m_pProductView[idx]->m_pScope;
	delete m_pProductView[idx]->m_pWaterfall;

	if(m_pProductView[idx]->m_pStatic[0] != NULL)
	{
		m_pProductView[idx]->m_pStatic[0]->DestroyWindow();
		delete m_pProductView[idx]->m_pStatic[0];
	}
	if(m_pProductView[idx]->m_pStatic[1] != NULL)
	{
		m_pProductView[idx]->m_pStatic[1]->DestroyWindow();
		delete m_pProductView[idx]->m_pStatic[1];
	}

	delete m_pProductView[idx];
	m_pProductView[idx] = NULL;
	ReLayoutProductView();
	return TRUE;
}

int CProductBaseView::GetIdealProductElement()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
		if(m_pProductView[i] == NULL)
			return i;
	return -1;
}

int CProductBaseView::GetValidProductElementCount()
{
	int sum = 0;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
		if(m_pProductView[i] != NULL)
			sum++;

	return sum;
}

BOOL CProductBaseView::IsProductViewExist(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	//int sum = 0;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == product
			&& m_pProductView[i]->m_Ch == ch)
			return TRUE;
	}

	return FALSE;
}

int CProductBaseView::GetProductViewIdx(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	int sum = 0;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == product
			&& m_pProductView[i]->m_Ch == ch)
			return i;
	}

	return -1;
}

void CProductBaseView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_tStart = CTime::GetCurrentTime();
	m_bInitial = TRUE;
}

void CProductBaseView::OnPaint()
{
	CPaintDC dc(this);

	CRect rt;
	GetClientRect(&rt);
	dc.FillSolidRect(rt,RGB(0,0,0));
}

BOOL CProductBaseView::AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	return TRUE;
}

void CProductBaseView::StartRealTimeView(CAcquireHelper* pAcquireHelper)
{
	m_heightInterval = pAcquireHelper->GetHeightScaleFactor();
	m_datetimeInterval = pAcquireHelper->GetDtScaleFactor();
	m_curvePointsSize = pAcquireHelper->GetChannelDistCount();

	m_tSpan = m_datetimeInterval * m_CurvesPerScreen;
}

void CProductBaseView::ReLayoutProductView()
{

}

CString CProductBaseView::BuildScreenImgFolder(CTime &tInfo,CProductHelper::ProductType productType)
{
	CString strFolder;
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	
	strFolder.Format(_T("\\%d年"),tInfo.GetYear());
	strFolder = pDoc->m_SavePath + strFolder;
	if(!PathFileExists(strFolder))
		CreateDirectory(strFolder,NULL);

	strFolder.Format(_T("\\%d年\\%02d月"),tInfo.GetYear(),tInfo.GetMonth());
	strFolder = pDoc->m_SavePath + strFolder;
	if(!PathFileExists(strFolder))
		CreateDirectory(strFolder,NULL);

	strFolder.Format(_T("\\%d年\\%02d月\\%02d日"),tInfo.GetYear(),tInfo.GetMonth(),tInfo.GetDay());
	strFolder = pDoc->m_SavePath + strFolder;
	if(!PathFileExists(strFolder))
		CreateDirectory(strFolder,NULL);

	if(productType == CProductHelper::ProductType_OrginSignal)
	{
		strFolder = strFolder + _T("\\原始数据");
		if(!PathFileExists(strFolder))
			CreateDirectory(strFolder,NULL);

		strFolder = strFolder + _T("\\LIDAR");
		if(!PathFileExists(strFolder))
			CreateDirectory(strFolder,NULL);

		return strFolder;
	}
	else if(productType < CProductHelper::ProductType_Cloud)
	{

	}
	else
	{

	}
}

void CProductBaseView::SaveScreenImg()
{
	/*CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	ImgObsFileNameParser *pImgObsFileNameParser = new ImgObsFileNameParser();

	pImgObsFileNameParser->_Time1ST = m_tStart;
	pImgObsFileNameParser->_Time2ST = CTime::GetCurrentTime();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		pImgObsFileNameParser->_CapMode = pDoc->m_AcquireHelper.GetMode();
		pImgObsFileNameParser->_WaveLength = CProductHelper::CHANNEL_WAVE_LENGTH[m_pProductView[i]->m_Ch];
		pImgObsFileNameParser->_WaveState = CProductHelper::CHANNEL_WAVE_STATE[m_pProductView[i]->m_Ch];

		switch(pDoc->m_AcquireHelper.GetMode())
		{
			case CAcquireHelper::AcquireMode_Fixed:
				pImgObsFileNameParser->_AziS = pDoc->m_ServoCtlHelper.m_pProperty->m_engine.GetFwAngle();
				pImgObsFileNameParser->_EleS = pDoc->m_ServoCtlHelper.m_pProperty->m_engine.GetFyAngle();
				pImgObsFileNameParser->_AccTime = pDoc->m_AcquireHelper.GetDtScaleFactor();
			break;
			case CAcquireHelper::AcquireMode_PPI:

			break;
			case CAcquireHelper::AcquireMode_RHI:

			break;
			default:
			break;
		}

	}*/
}