// RadarFormView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "RadarFormView.h"
#include "AxisSettingsDlg.h"
#include "Radar_wspDoc.h"
#include "MainFrm.h"
#include <fstream>
#include <iostream>
using namespace std;

IMPLEMENT_DYNCREATE(CRadarFormView, CProductBaseView)

CRadarFormView::CRadarFormView()
	: CProductBaseView(/*CRadarFormView::IDD*/)
{
}

CRadarFormView::~CRadarFormView()
{
}

void CRadarFormView::DoDataExchange(CDataExchange* pDX)
{
	CProductBaseView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRadarFormView, CProductBaseView)
	ON_WM_SIZE()
	ON_COMMAND(ID_AXISCONFIGURE_HORIZONTALAXIS, &CRadarFormView::OnAxisconfigureHorizontalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_VERTICALAXIS, &CRadarFormView::OnAxisconfigureVerticalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_LEVELAXIS, &CRadarFormView::OnAxisconfigureLevelaxis)
	ON_COMMAND(ID_AXISCONFIGURE_AXISALLRESET, &CRadarFormView::OnAxisconfigureAixsAllReset)
	ON_COMMAND(ID_AXISCONFIGURE_EXPORTIMG, &CRadarFormView::OnAxisconfigureExportImg)
END_MESSAGE_MAP()


void CRadarFormView::OnInitialUpdate()
{
	CProductBaseView::OnInitialUpdate();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	AddProductView(CProductHelper::ProductType_OrginSignal,1);
	AddProductView(CProductHelper::ProductType_OrginSignal,3);
	AddProductView(CProductHelper::ProductType_OrginSignal,5);
	m_bInitial = TRUE;
}

void CRadarFormView::ScrollProductView()
{
	ClearViewData();
	m_tSpan = 100*m_datetimeInterval;
	m_tStart = CTime::GetCurrentTime();
	
	//for(unsigned int i=0;i<CProductMgr::MAX_PRODUCT_VIEW;i++)
	//{
	//	if(m_pProductView[i] == NULL)
	//		continue;

	//	m_pProductView[i]->m_pWaterfall->XAxis.Min.DataValue = 0;
	//	m_pProductView[i]->m_pWaterfall->XAxis.Min.Value = 0;
	//	//m_pProductView[i]->m_pWaterfall->PostRefresh();
	//	//m_pProductView[i]->m_pWaterfall->XAxis.Max.DataValue = m_tSpan/pDoc->m_DtScaleFactor;
	//	//m_pProductView[i]->m_pWaterfall->XAxis.Max.Value = m_tSpan/pDoc->m_DtScaleFactor;
	//	//m_pProductView[i]->m_pWaterfall->XAxis.Samples = m_tSpan/pDoc->m_DtScaleFactor;
	//}
}

void CRadarFormView::ScrollProductView(CTime &tStart,CTime &tEnd)
{
	ClearViewData();
	m_tStart = tStart;
	CTimeSpan t = tEnd - tStart;
	//double tSpan = t.GetTotalSeconds();
	//for(unsigned int i=0;i<CProductMgr::MAX_PRODUCT_VIEW;i++)
	//{
	//	if(m_pProductView[i] == NULL)
	//		continue;

	//	m_pProductView[i]->m_pWaterfall->XAxis.Min.DataValue = 0;
	//	m_pProductView[i]->m_pWaterfall->XAxis.Min.Value = 0;

	//	//m_pProductView[i]->m_pWaterfall->XAxis.Max.DataValue = tSpan/pDoc->m_DtScaleFactor;
	//	//m_pProductView[i]->m_pWaterfall->XAxis.Max.Value = tSpan/pDoc->m_DtScaleFactor;
	//	//m_pProductView[i]->m_pWaterfall->XAxis.Samples = tSpan/pDoc->m_DtScaleFactor;
	//}
}

BOOL CRadarFormView::AddProductView(CProductHelper::ProductType product,unsigned int ch)
{
	if(IsProductViewExist(product,ch))
		return TRUE;

	int nIdx = GetIdealProductElement();
	if(nIdx == -1)
		return FALSE;

	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	m_pProductView[nIdx] = new ProductView;
	m_pProductView[nIdx]->m_ProductType = product;
	m_pProductView[nIdx]->m_Ch = ch;

	m_pProductView[nIdx]->m_pStatic[0] = new CStatic();
	m_pProductView[nIdx]->m_pStatic[1] = new CStatic();
	m_pProductView[nIdx]->m_pStatic[0]->Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,300,300),this,0);
	m_pProductView[nIdx]->m_pStatic[1]->Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,300,300),this,0);
	//时空伪彩图
	m_pProductView[nIdx]->m_pWaterfall = new CTSLWaterfall();
	m_pProductView[nIdx]->m_pWaterfall->Open(m_pProductView[nIdx]->m_pStatic[0]->m_hWnd);
	m_pProductView[nIdx]->m_pWaterfall->Align = CTAlign::alClient;
	m_pProductView[nIdx]->m_pWaterfall->ToolBar.Visible = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->Title.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pWaterfall->Title.Font.Size = 10;
	CString title(CProductHelper::PRODUCT_NAME[product]);
	if(ch != 0)
	{
		CString strTemp(CProductHelper::CHANNEL_NAME[ch-1]);
		title += _T("-");
		title += strTemp;
	}
	m_pProductView[nIdx]->m_pWaterfall->Title.Text = title;
	m_pProductView[nIdx]->m_pWaterfall->Title.Align = vaLeft;
	
	m_pProductView[nIdx]->m_pWaterfall->OnMouseMove.Set(this,&CRadarFormView::OnTSLMouseMove);
	m_pProductView[nIdx]->m_pWaterfall->OnMouseUp.Set(this,&CRadarFormView::OnTSLMouseUp);
	m_pProductView[nIdx]->m_pWaterfall->OnDataToolTip.Set(this,&CRadarFormView::OnTSLDataToolTip);
	// 配置色条坐标
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Autoscale = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Max = 3000;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Min = 0;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Add(5);
	m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Delete(0);
	// 配置时间坐标
	m_pProductView[nIdx]->m_pWaterfall->XAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->XAxis.Min.Value = 0;
	m_pProductView[nIdx]->m_pWaterfall->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pWaterfall->XAxis.AxisLabel.Text = _T("ʱ��");
	m_pProductView[nIdx]->m_pWaterfall->XAxis.OnCustomLabel.Set(this,&CRadarFormView::OnCustomDtAxesLabel);
	//配置高度坐标
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.DataValue = 0;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.Value = 0;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.DataValue = m_curvePointsSize;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.Value = m_curvePointsSize;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pWaterfall->YAxis.AxisLabel.Text = _T("�߶�(m)");
	m_pProductView[nIdx]->m_pWaterfall->YAxis.OnCustomLabel.Set(this,&CRadarFormView::OnCustomHeightAxesLabel);
	//曲线图
	m_pProductView[nIdx]->m_pScope = new CTSLScope();
	m_pProductView[nIdx]->m_pScope->Open(m_pProductView[nIdx]->m_pStatic[1]->m_hWnd);
	m_pProductView[nIdx]->m_pScope->Align = CTAlign::alClient;
	m_pProductView[nIdx]->m_pScope->ToolBar.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->Title.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pScope->Title.Text = _T("");
	m_pProductView[nIdx]->m_pScope->OnMouseUp.Set(this,&CRadarFormView::OnTSLMouseUp);
	
	//配置值坐标轴
	m_pProductView[nIdx]->m_pScope->XAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.Min.DataValue = 0;
	m_pProductView[nIdx]->m_pScope->XAxis.Min.Value = 0;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.DataValue = 42767;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.Value = 42767;
	m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pScope->XAxis.DataView.Lines.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.DataView.ZeroLine.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Visible = FALSE;

	//配置高度坐标轴
	m_pProductView[nIdx]->m_pScope->YAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.Min.DataValue = 0;
	m_pProductView[nIdx]->m_pScope->YAxis.Min.Value = 0;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.DataValue = 30000;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.Value = 30000;
	m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pScope->YAxis.DataView.Lines.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.DataView.ZeroLine.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Text = _T("�߶�(m)");
	
	m_pProductView[nIdx]->m_pScope->Legend.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->Channels.Add();

	VCL_Loaded();
	ReLayoutProductView();

	return TRUE;
}

void CRadarFormView::OnSize(UINT nType, int cx, int cy)
{
	CProductBaseView::OnSize(nType, cx, cy);

	if(!m_bInitial)
		return;

	if(GetValidProductElementCount() == 0)
	{
		CPaintDC dc(this);
		dc.FillSolidRect(CRect(0,0,cx,cy),RGB(0,0,0));
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText(_T("��ѡ�����ݲ�Ʒ�Թ�չʾ..."),14,
						CRect(0,0,cx,cy),DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		return;
	}

	int nDy = (cy-20) / GetValidProductElementCount();
	int y = 10;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx*3/4-10,nDy);
		m_pProductView[i]->m_pStatic[1]->MoveWindow(cx*3/4+10,y,cx/4-20,nDy);
		m_pProductView[i]->m_pWaterfall->Width = cx*3/4-10;
		m_pProductView[i]->m_pWaterfall->Height = nDy;
		m_pProductView[i]->m_pScope->Width = cx/4-20;
		m_pProductView[i]->m_pScope->Height = nDy;
		y += nDy;
	}
}

void CRadarFormView::ReLayoutProductView()
{
	CRect rt;
	GetClientRect(&rt);
	
	if(GetValidProductElementCount() == 0)
	{
		CPaintDC dc(this);
		dc.FillSolidRect(rt,RGB(0,0,0));
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText(_T("��ѡ�����ݲ�Ʒ�Թ�չʾ..."),14,
						rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		return;
	}

	int cx = rt.Width();
	int cy = rt.Height();

	int nDy = (cy-20) / GetValidProductElementCount();
	int y = 10;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx*3/4-10,nDy);
		m_pProductView[i]->m_pStatic[1]->MoveWindow(cx*3/4+10,y,cx/4-20,nDy);
		m_pProductView[i]->m_pWaterfall->Width = cx*3/4-10;
		m_pProductView[i]->m_pWaterfall->Height = nDy;
		m_pProductView[i]->m_pScope->Width = cx/4-20;
		m_pProductView[i]->m_pScope->Height = nDy;
		y += nDy;
	}
}

void __stdcall CRadarFormView::OnCustomDtAxesLabel(VCLHANDLE Sender, double Value, CString &label )
{
	CString s;
	CTime t = m_tStart + Value * m_datetimeInterval;
	s = t.Format(_T("%Y/%m/%d-%H:%M:%S"));
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_pWaterfall->XAxis.Get_VCL_Handle() != Sender)
			continue;
		m_pProductView[i]->m_pWaterfall->XAxis.SetTickText(s);
		break;
	}
}

void __stdcall CRadarFormView::OnCustomHeightAxesLabel(VCLHANDLE Sender, double Value, CTString &label)
{
	CString axisLabel;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_pWaterfall->YAxis.Get_VCL_Handle() != Sender)
			continue;

		axisLabel.Format(_T("%d"),(unsigned int)(Value * m_heightInterval));
		m_pProductView[i]->m_pWaterfall->YAxis.SetTickText(axisLabel);
		break;
	}
}

void __stdcall CRadarFormView::OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y)
{
	if(btn != mbRight)
		return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_AXISMENU));
	CMenu *pContextMenu = menu.GetSubMenu(0);
	//CBitmap contextMenuBitmap;
	//contextMenuBitmap.LoadBitmap(IDB_CONTEXTMENU);
	//pContextMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&contextMenuBitmap,&contextMenuBitmap);

	CPoint pt;
	GetCursorPos(&pt);
	CString chName1(CProductHelper::CHANNEL_NAME[0]);
	CString chName2(CProductHelper::CHANNEL_NAME[1]);
	CString chName3(CProductHelper::CHANNEL_NAME[2]);
	CString szText;
	int nMenus = pContextMenu->GetMenuItemCount();
	for(int j=0;j<nMenus;j++)
	{
		pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
		if(szText == chName1 || szText == chName2 || szText == chName3)
		{
			pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			continue;
		}
	}

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
		{
			m_pSelectedWaterfall = m_pProductView[i]->m_pWaterfall;
			m_pSelectedScope = NULL;
			for(int j=0;j<nMenus;j++)
			{
				pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
				if(szText == _T("X��"))
					pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
			break;
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			m_pSelectedWaterfall = NULL;
			m_pSelectedScope = m_pProductView[i]->m_pScope;

			for(int j=0;j<nMenus;j++)
			{
				pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
				if(szText == _T("ɫ��"))
					pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			}
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
			break;
		}
	}
}

void __stdcall CRadarFormView::OnTSLMouseMove(VCLHANDLE Sender,CTShiftState shift,int x,int y)
{
	CString strToolTipText;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;
	
		if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
		{	
			strToolTipText = m_pProductView[i]->m_pWaterfall->GetToolTipText();
			pMainFrame->UpdateCursorPos(strToolTipText);
			break;
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			strToolTipText = m_pProductView[i]->m_pScope->GetToolTipText();
			pMainFrame->UpdateCursorPos(strToolTipText);
			break;
		}
	}
}

void __stdcall CRadarFormView::OnTSLDataToolTip(VCLHANDLE Sender,POINT &pt,CTSLRealPoint &realPt,CTString &tooltip)
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	CString strToolTip;
	CString strDataVal;
	CString sTime;

	CTime t = m_tStart + realPt.X * m_datetimeInterval;
	sTime = t.Format(_T("%Y/%m/%d-%H:%M:%S "));

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
		{
			strToolTip.Format(_T("��λ�Ƕ�:270.90 �����Ƕ�:28.89 �߶�:%.0f "),realPt.Y * m_heightInterval);
			CString strDataVal;
			CString strDataFormat(CProductHelper::PRODUCT_NAME_FORMAT[m_pProductView[i]->m_ProductType]);
			strDataVal.Format(strDataFormat,m_pProductView[i]->m_pWaterfall->DataView.GetValueAtScale(realPt.X,realPt.Y));
			strToolTip = sTime + strToolTip;
			strToolTip += strDataVal;
			m_pProductView[i]->m_pWaterfall->SetToolTipText(strToolTip);
			break;
		}
		else
		{
		}
	}
}

void CRadarFormView::UpdateView(CDatModel* pModel)
{
	ASSERT(pModel);

	unsigned int sum = pModel->m_pRecords->size();
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	CDatRecord* pRecord = pModel->m_pRecords->at(0);
	m_datetimeInterval = pRecord->m_FramePeriod;
	m_heightInterval = CAcquireHelper::CalcHeightResolution(pRecord->m_SampleFreq);
	m_curvePointsSize = pRecord->m_SampleNum;

	ScrollProductView(pModel->m_pRecords->at(0)->m_Time,pModel->m_pRecords->at(sum-1)->m_Time);

	unsigned int arraylen = 0;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end())
			{
				arraylen = (*it)->m_pSamples->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pSamples->m_pData[m_pProductView[i]->m_Ch],arraylen);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

			it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end())
			{
				arraylen = (*it)->m_pExtinctionCoefficient->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch],arraylen);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

			it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end())
			{
				arraylen = (*it)->m_pDeplorRatio->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pDeplorRatio->m_pData[1],arraylen);
				it++;
			}
		}
	}
}

void CRadarFormView::UpdateView(CDatModel* pModel,CProductHelper::ProductType product,unsigned int ch)
{
	ASSERT(pModel);

	// unsigned int idx;
	// unsigned int sum = pModel->m_pRecords->size();
	// if(sum <= 1)
	// 	return;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//unsigned int len = pModel->m_pRecords->at(0)->m_pSamples->GetLength();
	//vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType != product)
			continue;

		if(m_pProductView[i]->m_Ch != ch)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			std::vector<CDatRecord*>::iterator it = pModel->m_pLidarRecords->begin();
			while(it != pModel->m_pLidarRecords->end() - 1)
			{
				m_pProductView[i]->m_pWaterfall->Data.AddData(
							(*it)->m_channles[m_pProductView[i]->m_Ch - 1].m_pSample->m_pData[0],
							m_curvePointsSize);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			// idx = 0;
			// pMainFrame->UpdateWorkFlowState(0,sum);
			// BeginWaitCursor();
			// while(it != pModel->m_pRecords->end() - 1)
			// {
			// 	if((*it)->m_pExtinctionCoefficient == NULL)
			// 		CProductHelper::Extinction((*it));
			// 	idx++;
			// 	it++;
			// 	pMainFrame->UpdateWorkFlowState(IDS_WORKFLOW_EXTINCCOE,idx);
			// }
			// EndWaitCursor();
			// pMainFrame->UpdateWorkFlowState(0,0);

			// it = pModel->m_pRecords->begin();
			// while(it != pModel->m_pRecords->end() - 1)
			// {
			// 	m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch],len);
			// 	it++;
			// }
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			// idx = 0;
			// pMainFrame->UpdateWorkFlowState(0,sum);
			// BeginWaitCursor();
			// while(it != pModel->m_pRecords->end())
			// {
			// 	if((*it)->m_pDeplorRatio == NULL)
			// 		CProductHelper::Depolar((*it));
			// 	idx++;
			// 	it++;
			// 	pMainFrame->UpdateWorkFlowState(IDS_WORKFLOW_DEPOLAR,idx);
			// }
			// EndWaitCursor();
			// pMainFrame->UpdateWorkFlowState(0,0);

			// it = pModel->m_pRecords->begin();
			// while(it != pModel->m_pRecords->end())
			// {
			// 	m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pDeplorRatio->m_pData[1],len);
			// 	it++;
			// }
		}
	}
}

void CRadarFormView::UpdateViewFlyBy(CDatModel* pModel/* CDatRecord* pRecord */,BOOL bWaterFall)
{
	ASSERT(pModel);
	unsigned int idxFlyby = pModel->m_pLidarRecords->size();
	ASSERT(idxFlyby)
	
	double *nullBuf = new double[m_curvePointsSize];
	memset(nullBuf,0x00,sizeof(double) * m_curvePointsSize);

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			CDatRecord* pLidarRecord = pModel->m_pLidarRecords->at(idxFlyby - 1);
			m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(
								pLidarRecord->m_channels[m_pProductView[i]->m_Ch-1]->m_pSample->m_pData[0],
								m_pRange,
								m_curvePointsSize);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			// if(pRecord->m_pExtinctionCoefficient == NULL)
			// 	m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(nullBuf,pRange,len);
			// else m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch],pRange,len);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			// if(pRecord->m_pDeplorRatio == NULL)
			// 	m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(nullBuf,pRange,len);
			// else m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pDeplorRatio->m_pData[1],pRange,len);
		}
	}

	if(!bWaterFall)
		return;

	if(pRecord->m_Time - m_tStart > m_tSpan)
		ScrollProductView();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			CDatRecord* pLidarRecord = pModel->m_pLidarRecords->at(idxFlyby - 1);
			m_pProductView[i]->m_pWaterfall->Data.AddData(
									pLidarRecord->m_channels[m_pProductView[i]->m_Ch-1]->m_pSample->m_pData[0],
									m_curvePointsSize);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			// if(pRecord->m_pExtinctionCoefficient == NULL)
			// 	m_pProductView[i]->m_pWaterfall->Data.AddData(nullBuf,len);
			// else m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch],len);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			// if(pRecord->m_pDeplorRatio == NULL)
			// 	m_pProductView[i]->m_pWaterfall->Data.AddData(nullBuf,len);
			// else m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pDeplorRatio->m_pData[1],len);
		}
	}
}

void CRadarFormView::StartRealTimeView(CAcquireHelper* pAcquireHelper)
{
	m_heightInterval = pAcquireHelper->GetHeightScaleFactor();
	m_datetimeInterval = pAcquireHelper->GetDtScaleFactor();
	m_curvePointsSize = pAcquireHelper->GetChannelDistCount();
	m_pRange = pAcquireHelper->GetRange();
	
	ScrollProductView();
}

void CRadarFormView::OnAxisconfigureHorizontalaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	if(m_pSelectedScope != NULL)
	{
		dlg.m_Max = m_pSelectedScope->XAxis.CurrentMax;
		dlg.m_Min = m_pSelectedScope->XAxis.CurrentMin;
		dlg.m_Title = _T("ˮƽ����������");
		dlg.m_bLogCheckEnable = TRUE;
		dlg.m_bLogEnable = (m_pSelectedScope->XAxis.ScaleMode == smLogarithmic);
		if(IDOK != dlg.DoModal())
			return;

		m_pSelectedScope->XAxis.ScaleMode = dlg.m_bLogEnable ? smLogarithmic : smLinear;

		m_pSelectedScope->XAxis.ZoomTo(dlg.m_Min,dlg.m_Max,FALSE);
		m_pSelectedScope->Hold = FALSE;
	}
	else return;
}

void CRadarFormView::OnAxisconfigureVerticalaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	if(m_pSelectedWaterfall != NULL)
	{
		dlg.m_Max = m_pSelectedWaterfall->YAxis.CurrentMax * m_heightInterval;
		dlg.m_Min = m_pSelectedWaterfall->YAxis.CurrentMin * m_heightInterval;
		dlg.m_Title = _T("��ֱ����������(�߶�/m)");
		dlg.m_bLogCheckEnable = FALSE;
		if(IDOK != dlg.DoModal())
			return;

		m_pSelectedWaterfall->YAxis.ZoomTo(dlg.m_Min / m_heightInterval,dlg.m_Max / m_heightInterval);
		m_pSelectedWaterfall->Hold = FALSE;
	}
	else if(m_pSelectedScope != NULL)
	{
		dlg.m_Max = m_pSelectedScope->YAxis.CurrentMax;
		dlg.m_Min = m_pSelectedScope->YAxis.CurrentMin;
		dlg.m_Title = _T("��ֱ����������(�߶�/m)");
		dlg.m_bLogCheckEnable = FALSE;
		if(IDOK != dlg.DoModal())
			return;

		m_pSelectedScope->YAxis.ZoomTo(dlg.m_Min,dlg.m_Max,FALSE);
		m_pSelectedScope->Hold = FALSE;
	}
	else return;
}

void CRadarFormView::OnAxisconfigureLevelaxis()
{
	CAxisSettingsDlg dlg;
	if(m_pSelectedWaterfall != NULL)
	{
		dlg.m_Max = m_pSelectedWaterfall->Levels.Axis.Max;
		dlg.m_Min = m_pSelectedWaterfall->Levels.Axis.Min;
		dlg.m_Title = _T("ɫ������������");
		dlg.m_bLogEnable = (m_pSelectedWaterfall->Levels.Axis.ScaleMode == smLogarithmic);
		dlg.m_bLogCheckEnable = TRUE;
		if(IDOK != dlg.DoModal())
			return;

		m_pSelectedWaterfall->Levels.Axis.Max = dlg.m_Max;
		m_pSelectedWaterfall->Levels.Axis.Min = dlg.m_Min;
		m_pSelectedWaterfall->Levels.Axis.ScaleMode = dlg.m_bLogEnable ? smLogarithmic : smLinear;
	}
}

void CRadarFormView::OnAxisconfigureAixsAllReset()
{
	CAxisSettingsDlg dlg;
	if(m_pSelectedWaterfall != NULL)
	{
		m_pSelectedWaterfall->Hold = FALSE;
	}
	else if(m_pSelectedScope != NULL)
	{
		m_pSelectedScope->Hold = FALSE;
	}
	else return;
}

void CRadarFormView::OnAxisconfigureExportImg()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	CFileDialog f(FALSE);
	if(f.DoModal() != IDOK)
		return;

	if(m_pSelectedWaterfall != NULL)
	{
		m_pSelectedWaterfall->SaveToBitmap(f.GetPathName());
	}
	else if(m_pSelectedScope != NULL)
	{
		m_pSelectedWaterfall->SaveToBitmap(f.GetPathName());
	}
	else return;	
}
