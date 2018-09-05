// ProductBaseView.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "ProductBaseView.h"


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

		//m_pProductView[i]->m_pWaterfall->Data.Clear();
	}
}

BOOL CProductBaseView::DelProductView(CProductHelper::ProductType product,unsigned int ch)
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

BOOL CProductBaseView::IsProductViewExist(CProductHelper::ProductType product,unsigned int ch)
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

int CProductBaseView::GetProductViewIdx(CProductHelper::ProductType product,unsigned int ch)
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

BOOL CProductBaseView::AddProductView(CProductHelper::ProductType product,unsigned int ch)
{
	return TRUE;
}

void CProductBaseView::ReLayoutProductView()
{

}