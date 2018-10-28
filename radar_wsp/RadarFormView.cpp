// RadarFormView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "RadarFormView.h"
#include "AxisSettingsDlg.h"
#include "Radar_wspDoc.h"
#include "MainFrm.h"
#include "RecordDlg.h"

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
	ON_COMMAND(ID_AXISCONFIGURE_PRINT, &CRadarFormView::OnAxisconfigurePrint)
	ON_COMMAND(ID_AXISCONFIGURE_TABLE, &CRadarFormView::OnAxisconfigureTable)
END_MESSAGE_MAP()


void CRadarFormView::OnInitialUpdate()
{
	CProductBaseView::OnInitialUpdate();

	AddProductView(CProductHelper::ProductType_OrginSignal,CProductHelper::ProductChannelType_1ch);
	AddProductView(CProductHelper::ProductType_OrginSignal,CProductHelper::ProductChannelType_2ch);
	AddProductView(CProductHelper::ProductType_OrginSignal,CProductHelper::ProductChannelType_3ch);

	m_CurvesPerScreen = m_pProductView[0]->m_pWaterfall->XAxis.CurrentMax;

	m_bInitial = TRUE;
}

void CRadarFormView::ScrollProductView()
{
	ClearViewData();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	if(pDoc->m_ProductHelper.GetCount() != 0)
		pDoc->m_ProductHelper.Remove(pDoc->m_ProductHelper.GetCount() - 1);
	
	m_tStart = CTime::GetCurrentTime();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		m_pProductView[i]->m_pWaterfall->YAxis.Max.DataValue = m_curvePointsSize;
		m_pProductView[i]->m_pWaterfall->YAxis.Max.Value = m_curvePointsSize;

		m_pProductView[i]->m_pScope->YAxis.Max.DataValue = m_curvePointsSize * m_heightInterval;
		m_pProductView[i]->m_pScope->YAxis.Max.Value = m_curvePointsSize * m_heightInterval;

		m_pProductView[i]->m_pWaterfall->Invalidate();
	}
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

BOOL CRadarFormView::AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	if(IsProductViewExist(product,ch))
		return TRUE;

	int nIdx = GetIdealProductElement();
	if(nIdx == -1)
		return FALSE;

	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisConfigHelper::PTR_AxisSettings pAxisSettings = pDoc->m_AxisConfigHelper.GetProductConfig(product,ch);

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
	m_pProductView[nIdx]->m_pWaterfall->Title.Font.Size = 11;
	CString title(CProductHelper::PRODUCT_NAME[product]);
	CString strTemp(CProductHelper::CHANNEL_NAME[ch]);
	title += _T("-");
	title += strTemp;
	m_pProductView[nIdx]->m_pWaterfall->Title.Text = title;
	m_pProductView[nIdx]->m_pWaterfall->Title.Align = vaLeft;
	
	m_pProductView[nIdx]->m_pWaterfall->OnMouseMove.Set(this,&CRadarFormView::OnTSLMouseMove);
	m_pProductView[nIdx]->m_pWaterfall->OnMouseUp.Set(this,&CRadarFormView::OnTSLMouseUp);
	m_pProductView[nIdx]->m_pWaterfall->OnDataToolTip.Set(this,&CRadarFormView::OnTSLDataToolTip);
	// 配置色条坐标
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Autoscale = FALSE;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Max = pAxisSettings->level_max;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Min = pAxisSettings->level_min;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.ScaleMode = pAxisSettings->log_c ? smLogarithmic : smLinear;
	m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Add(5);
	m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Delete(0);
	m_pProductView[nIdx]->m_pWaterfall->Levels.Visible = pAxisSettings->level_visible;

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
	m_pProductView[nIdx]->m_pScope->Title.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->OnMouseUp.Set(this,&CRadarFormView::OnTSLMouseUp);
	
	//配置值坐标轴
	m_pProductView[nIdx]->m_pScope->XAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.Min.DataValue = pAxisSettings->default_min;
	m_pProductView[nIdx]->m_pScope->XAxis.Min.Value = pAxisSettings->default_min;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.DataValue = pAxisSettings->default_max;
	m_pProductView[nIdx]->m_pScope->XAxis.Max.Value = pAxisSettings->default_max;
	m_pProductView[nIdx]->m_pScope->XAxis.ScaleMode = pAxisSettings->log_t ? smLogarithmic : smLinear;
	m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pScope->XAxis.DataView.Lines.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.DataView.ZeroLine.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Visible = FALSE;

	//配置高度坐标轴
	m_pProductView[nIdx]->m_pScope->YAxis.Min.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.Min.DataValue = 0;
	m_pProductView[nIdx]->m_pScope->YAxis.Min.Value = 0;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.AutoScale = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.DataValue = m_curvePointsSize * m_heightInterval;
	m_pProductView[nIdx]->m_pScope->YAxis.Max.Value = m_curvePointsSize * m_heightInterval;
	m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
	m_pProductView[nIdx]->m_pScope->YAxis.DataView.Lines.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.DataView.ZeroLine.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Text = _T("�߶�(m)");
	
	m_pProductView[nIdx]->m_pScope->Legend.Visible = FALSE;
	m_pProductView[nIdx]->m_pScope->Channels.Add();

	VCL_Loaded();
	ReLayoutProductView();

	m_pProductView[nIdx]->m_pScope->XAxis.ZoomTo(pAxisSettings->min,pAxisSettings->max,FALSE);
	m_pProductView[nIdx]->m_pScope->Hold = FALSE;

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
	CString szText;
	int nMenus = pContextMenu->GetMenuItemCount();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
		{
			m_pSelectedWaterfall = m_pProductView[i]->m_pWaterfall;
			m_pSelectedScope = NULL;
			m_SelectProductType = m_pProductView[i]->m_ProductType;
			m_SelectChannelType = m_pProductView[i]->m_Ch;

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
			m_SelectProductType = m_pProductView[i]->m_ProductType;
			m_SelectChannelType = m_pProductView[i]->m_Ch;

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

	/*ËÑË÷Ê±¼ä¶ÔÓ¦µÄ¼ÇÂ¼*/
	CProductHelper *pProductHelper = &(pDoc->m_ProductHelper);
	m_pHookedRecord = pProductHelper->SearchRecord(t);
	double fy = 0,fw = 0;
	if(m_pHookedRecord != NULL)
	{
		fy = m_pHookedRecord->m_FyAngle;
		fw = m_pHookedRecord->m_FwAngle;
	}

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
		{
			strToolTip.Format(_T("·½Î»½Ç¶È:%.2f ¸©Ñö½Ç¶È:%.2f ¸ß¶È:%.0f "),
								fw,
								fy,
								realPt.Y * m_heightInterval);
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
	
}

void CRadarFormView::UpdateView(CDatModel* pModel,CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	ASSERT(pModel);

	unsigned int sum = pModel->m_pRecords->size();
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	unsigned int arraylen = 0;
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
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end()-1)
			{
				arraylen = (*it)->m_pSamples->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pRegular->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_BackScatter)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end()-1)
			{
				arraylen = (*it)->m_pSamples->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pBackScatter->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

			while(it != pModel->m_pRecords->end()-1)
			{
				arraylen = (*it)->m_pExtinctionCoefficient->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

			while(it != pModel->m_pRecords->end()-1)
			{
				arraylen = (*it)->m_pDeplorRatio->GetLength();
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pDeplorRatio->m_pData[1],arraylen);
				it++;
			}
		}
	}
}

void CRadarFormView::UpdateView(CDatRecord* pRecord,BOOL bWaterFall)
{
	ASSERT(pRecord);

	double *pRange = pRecord->m_pSamples->m_pData[0];

	unsigned int arraylen;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pSamples->m_pData[m_pProductView[i]->m_Ch+1],pRange,arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_BackScatter)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pBackScatter->m_pData[m_pProductView[i]->m_Ch+1],pRange,arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch+1],pRange,arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pScope->Channels[0].Data.SetXYData(pRecord->m_pDeplorRatio->m_pData[1],pRange,arraylen);
		}
	}

	if(!bWaterFall)
		return;

	if(pRecord->m_Time - m_tStart > m_tSpan)
	{
		SaveScreenImg();
		ScrollProductView();
	}

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OrginSignal)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pRegular->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_BackScatter)
		{
			arraylen = pRecord->m_pSamples->GetLength();
			m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pBackScatter->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_ExtinctionCoefficient)
		{
			arraylen = pRecord->m_pExtinctionCoefficient->GetLength();
			m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pExtinctionCoefficient->m_pData[m_pProductView[i]->m_Ch+1],arraylen);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_DepolarizationRatio)
		{
			arraylen = pRecord->m_pDeplorRatio->GetLength();
			m_pProductView[i]->m_pWaterfall->Data.AddData(pRecord->m_pDeplorRatio->m_pData[1],arraylen);
		}
	}
}

void CRadarFormView::StartRealTimeView(CAcquireHelper* pAcquireHelper)
{
	CProductBaseView::StartRealTimeView(pAcquireHelper);
	ScrollProductView();
}

void CRadarFormView::OnAxisconfigureHorizontalaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	CAxisSettingsDlg dlg; 
	if(m_pSelectedScope == NULL)
		return;

	dlg.m_Max = m_pSelectedScope->XAxis.CurrentMax;
	dlg.m_Min = m_pSelectedScope->XAxis.CurrentMin;
	dlg.m_Title = _T("Ë®Æ½×ø±êÖáÉèÖÃ");
	dlg.m_bLogCheckEnable = TRUE;
	dlg.m_bLogEnable = (m_pSelectedScope->XAxis.ScaleMode == smLogarithmic);
	if(IDOK != dlg.DoModal())
		return;

	CAxisConfigHelper::PTR_AxisSettings pAxisSettings = pDoc->m_AxisConfigHelper.GetProductConfig(m_SelectProductType,m_SelectChannelType);
	pAxisSettings->log_t = dlg.m_bLogEnable;
	pAxisSettings->min = dlg.m_Min;
	pAxisSettings->max = dlg.m_Max;
	pDoc->m_AxisConfigHelper.SaveModify(m_SelectProductType,m_SelectChannelType);

	m_pSelectedScope->XAxis.ScaleMode = pAxisSettings->log_t ? smLogarithmic : smLinear;
	m_pSelectedScope->XAxis.ZoomTo(pAxisSettings->min,pAxisSettings->max,FALSE);
	m_pSelectedScope->Hold = FALSE;
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
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	if(m_pSelectedWaterfall == NULL)
		return;

	dlg.m_Max = m_pSelectedWaterfall->Levels.Axis.Max;
	dlg.m_Min = m_pSelectedWaterfall->Levels.Axis.Min;
	dlg.m_Title = _T("É«Ìõ×ø±êÖáÉèÖÃ");
	dlg.m_bLogEnable = (m_pSelectedWaterfall->Levels.Axis.ScaleMode == smLogarithmic);
	dlg.m_bLogCheckEnable = TRUE;
	if(IDOK != dlg.DoModal())
		return;

	CAxisConfigHelper::PTR_AxisSettings pAxisSettings = pDoc->m_AxisConfigHelper.GetProductConfig(m_SelectProductType,m_SelectChannelType);
	pAxisSettings->log_c = dlg.m_bLogEnable;
	pAxisSettings->min = dlg.m_Min;
	pAxisSettings->max = dlg.m_Max;
	pDoc->m_AxisConfigHelper.SaveModify(m_SelectProductType,m_SelectChannelType);

	m_pSelectedWaterfall->Levels.Axis.Max = pAxisSettings->max;
	m_pSelectedWaterfall->Levels.Axis.Min = pAxisSettings->min;
	m_pSelectedWaterfall->Levels.Axis.ScaleMode = pAxisSettings->log_c ? smLogarithmic : smLinear;
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
	CFileDialog f(FALSE);
	if(f.DoModal() != IDOK)
		return;

	if(m_pSelectedWaterfall != NULL)
	{
		m_pSelectedWaterfall->SaveToBitmap(f.GetPathName());
	}
	else if(m_pSelectedScope != NULL)
	{
		m_pSelectedScope->SaveToBitmap(f.GetPathName());
	}
	else return;	
}

void CRadarFormView::OnAxisconfigurePrint()
{
	if(m_pSelectedWaterfall != NULL)
	{
		m_pSelectedWaterfall->Print();
	}
	else if(m_pSelectedScope != NULL)
	{
		m_pSelectedScope->Print();
	}
	else return;	
}

void CRadarFormView::OnAxisconfigureTable()
{
	if(m_pHookedRecord == NULL)
		return;

	CRecordDlg *pDlg = new CRecordDlg();
	pDlg->Create(IDD_RECORDDLG,this);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->CenterWindow();
	pDlg->DisplayRecord(m_SelectProductType,m_pHookedRecord);
}
