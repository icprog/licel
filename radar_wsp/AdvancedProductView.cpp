// AdvancedProductView.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "AdvancedProductView.h"
#include "Radar_wspDoc.h"
#include "AxisSettingsDlg.h"
#include "MainFrm.h"

unsigned int CAdvancedProductView::PRODUCT_CLOUD_COLOR[] = {0,300,300,300};

IMPLEMENT_DYNCREATE(CAdvancedProductView, CProductBaseView)

CAdvancedProductView::CAdvancedProductView()
	: CProductBaseView(/*CAdvancedProductView::IDD*/)
{

}

CAdvancedProductView::~CAdvancedProductView()
{
}

void CAdvancedProductView::DoDataExchange(CDataExchange* pDX)
{
	CProductBaseView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAdvancedProductView, CProductBaseView)
	ON_WM_SIZE()
	ON_COMMAND(ID_AXISCONFIGURE_CH1, &CAdvancedProductView::OnAxisconfigureCh1)
	ON_COMMAND(ID_AXISCONFIGURE_CH2, &CAdvancedProductView::OnAxisconfigureCh2)
	ON_COMMAND(ID_AXISCONFIGURE_CH3, &CAdvancedProductView::OnAxisconfigureCh3)
	ON_COMMAND(ID_AXISCONFIGURE_VERTICALAXIS, &CAdvancedProductView::OnAxisconfigureVerticalaxis)
END_MESSAGE_MAP()

void CAdvancedProductView::ReLayoutProductView()
{
	CRect rt;
	GetClientRect(&rt);

	if(GetValidProductElementCount() == 0)
	{
		CPaintDC dc(this);
		dc.FillSolidRect(rt,RGB(0,0,0));
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText(_T("请选择数据产品以供展示..."),14,
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

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx-10,nDy);
			m_pProductView[i]->m_pWaterfall->Width = cx-10;
			m_pProductView[i]->m_pWaterfall->Height = nDy;
		}
		else
		{
			m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx-10,nDy);
			m_pProductView[i]->m_pScope->Width = cx-10;
			m_pProductView[i]->m_pScope->Height = nDy;
		}	

		y += nDy;
	}
}

void CAdvancedProductView::OnSize(UINT nType, int cx, int cy)
{
	CProductBaseView::OnSize(nType, cx, cy);

	if(!m_bInitial)
		return;

	int nProductCount = GetValidProductElementCount();
	if(!nProductCount)
		return;

	int nDy = (cy-20) / nProductCount;
	int y = 10;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx-10,nDy);
			m_pProductView[i]->m_pWaterfall->Width = cx-10;
			m_pProductView[i]->m_pWaterfall->Height = nDy;
		}
		else
		{
			m_pProductView[i]->m_pStatic[0]->MoveWindow(10,y,cx-10,nDy);
			m_pProductView[i]->m_pScope->Width = cx-10;
			m_pProductView[i]->m_pScope->Height = nDy;
		}

		y += nDy;
	}
}

void CAdvancedProductView::OnInitialUpdate()
{
	CProductBaseView::OnInitialUpdate();

}

BOOL CAdvancedProductView::AddProduct(CProductHelper::ProductType product,unsigned int ch)
{
	BOOL bReLayout = FALSE;

	unsigned int nIdx = GetIdealProductElement();
	if(nIdx == -1)
		return FALSE;

	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	m_pProductView[nIdx] = new ProductView;
	m_pProductView[nIdx]->m_ProductType = product;
	m_pProductView[nIdx]->m_Ch = ch;	
	m_pProductView[nIdx]->m_pStatic[0] = new CStatic();
	m_pProductView[nIdx]->m_pStatic[0]->Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,300,300),this,0);

	CString title(CProductHelper::PRODUCT_NAME[product]);
	CString yAxisLabel(CProductHelper::PRODUCT_NAME_UNIT[product]);
	if(m_pProductView[nIdx]->m_ProductType == CProductHelper::ProductType_Cloud)
	{
		m_pProductView[nIdx]->m_pWaterfall = new CTSLWaterfall();
		m_pProductView[nIdx]->m_pWaterfall->Open(m_pProductView[nIdx]->m_pStatic[0]->m_hWnd);
		m_pProductView[nIdx]->m_pWaterfall->Align = CTAlign::alClient;
		m_pProductView[nIdx]->m_pWaterfall->ToolBar.Visible = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->Title.Font.Charset = GB2312_CHARSET;
		CString strTemp(CProductHelper::CHANNEL_NAME[ch-1]);
		title += _T("-");
		title += strTemp;
		m_pProductView[nIdx]->m_pWaterfall->Title.Text = title;
	
		m_pProductView[nIdx]->m_pWaterfall->OnMouseMove.Set(this,&CAdvancedProductView::OnTSLMouseMove);
		m_pProductView[nIdx]->m_pWaterfall->OnMouseUp.Set(this,&CAdvancedProductView::OnTSLMouseUp);//弹出上下文菜单
		m_pProductView[nIdx]->m_pWaterfall->OnDataToolTip.Set(this,&CAdvancedProductView::OnTSLDataToolTip);//自定义ToolTip
	
		m_pProductView[nIdx]->m_pWaterfall->Levels.Visible = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Autoscale = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Max = 300;
		m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Min = 0;

		m_pProductView[nIdx]->m_pWaterfall->XAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->XAxis.Min.Value = 0;
		m_pProductView[nIdx]->m_pWaterfall->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pWaterfall->XAxis.AxisLabel.Text = _T("时间");
		m_pProductView[nIdx]->m_pWaterfall->XAxis.OnCustomLabel.Set(this,&CAdvancedProductView::OnCustomDtAxesLabel);//自定义X轴显示日期时间
	
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.DataValue = 0;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Min.Value = 0;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.DataValue = m_curvePointsSize;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.Max.Value = m_curvePointsSize;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.AxisLabel.Text = yAxisLabel;
		m_pProductView[nIdx]->m_pWaterfall->YAxis.OnCustomLabel.Set(this,&CAdvancedProductView::OnCustomHeightAxesLabel);//自定义Y轴显示高度
	}
	else
	{
		m_pProductView[nIdx]->m_pScope = new CTSLScope();
		m_pProductView[nIdx]->m_pScope->Open(m_pProductView[nIdx]->m_pStatic[0]->m_hWnd);
		m_pProductView[nIdx]->m_pScope->Align = CTAlign::alClient;
		m_pProductView[nIdx]->m_pScope->ToolBar.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->Title.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pScope->Title.Text = title;
		m_pProductView[nIdx]->m_pScope->OnMouseMove.Set(this,&CAdvancedProductView::OnTSLMouseMove);
		m_pProductView[nIdx]->m_pScope->OnMouseUp.Set(this,&CAdvancedProductView::OnTSLMouseUp);//弹出上下文菜单
		m_pProductView[nIdx]->m_pScope->OnDataToolTip.Set(this,&CAdvancedProductView::OnTSLDataToolTip);//自定义ToolTip

		m_pProductView[nIdx]->m_pScope->XAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.Min.DataValue = 0;
		m_pProductView[nIdx]->m_pScope->XAxis.Min.Value = 0;
		//m_pProductView[nIdx]->m_pScope->XAxis.Max.AutoScale = FALSE;
		//m_pProductView[nIdx]->m_pScope->XAxis.Max.DataValue = 300;
		//m_pProductView[nIdx]->m_pScope->XAxis.Max.Value = 300;
		m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pScope->XAxis.DataView.Lines.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.DataView.ZeroLine.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Text = _T("时间");
		m_pProductView[nIdx]->m_pScope->XAxis.OnCustomLabel.Set(this,&CAdvancedProductView::OnCustomDtAxesLabel);//自定义X轴显示日期时间

		m_pProductView[nIdx]->m_pScope->YAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.Min.DataValue = 0;
		m_pProductView[nIdx]->m_pScope->YAxis.Min.Value = 0;
		//m_pProductView[nIdx]->m_pScope->YAxis.Max.AutoScale = FALSE;
		//m_pProductView[nIdx]->m_pScope->YAxis.Max.DataValue = 12000.0;
		//m_pProductView[nIdx]->m_pScope->YAxis.Max.Value = 12000.0;
		m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pScope->YAxis.DataView.Lines.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.DataView.ZeroLine.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Text = yAxisLabel;
	
		//FIXME:通道数固定
		m_pProductView[nIdx]->m_pScope->Channels.Clear();
		for(int i=0;i<CProductHelper::ProductChannelType_Sum;i++)
		{
			m_pProductView[nIdx]->m_pScope->Channels.Add();
			CString chName1(CProductHelper::CHANNEL_NAME[i]);
			m_pProductView[nIdx]->m_pScope->Channels[i].Name = chName1;
		}
	}
	
	VCL_Loaded();
	ReLayoutProductView();

	return TRUE;
}

void CAdvancedProductView::UpdateView(CDatModel* pModel)
{
	//CDatModel *pModel = ((CRadar_wspDoc *)GetDocument())->m_pModel;
	if(pModel == NULL)
		return;

	unsigned int idx,sum = pModel->m_pRecords->size();
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	ScrollProductView(pModel->m_pRecords->at(0)->m_Time,pModel->m_pRecords->at(sum-1)->m_Time);

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			//计算云信息
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();
			idx = 0;
			pMainFrame->UpdateWorkFlowState(0,sum);
			BeginWaitCursor();
			while(it != pModel->m_pRecords->end())
			{
				/*if((*it)->m_pCloud == NULL)
					pModel->ApplyCloudAlg((*it));*/
				idx++;
				it++;
				pMainFrame->UpdateWorkFlowState(IDS_WORKFLOW_EXTINCCOE,idx);
			}
			EndWaitCursor();
			pMainFrame->UpdateWorkFlowState(0,0);

			int samples = pModel->m_pRecords->at(0)->m_pSamples->GetLength();
			double* pCloud = new double[samples];
			it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end())
			{
				for(int j=0;j<samples;j++)
				{
					if((*it)->m_pCloud->m_pData[m_pProductView[i]->m_Ch][j])
						pCloud[j] = PRODUCT_CLOUD_COLOR[m_pProductView[i]->m_Ch];
					else pCloud[j] = PRODUCT_CLOUD_COLOR[0];
				}
				m_pProductView[i]->m_pWaterfall->Data.AddData(pCloud,samples);
				it++;
			}
			delete[] pCloud;
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OpticDepth)
		{
			vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();
			idx = 0;
			pMainFrame->UpdateWorkFlowState(0,sum);
			BeginWaitCursor();
			while(it != pModel->m_pRecords->end())
			{
			/*	if((*it)->m_pOpticDepth == NULL)
					pModel->ApplyOpticDepthAlg((*it));*/
				idx++;
				it++;
				pMainFrame->UpdateWorkFlowState(IDS_WORKFLOW_EXTINCCOE,idx);
			}
			EndWaitCursor();
			pMainFrame->UpdateWorkFlowState(0,0);

			it = pModel->m_pRecords->begin();
			while(it != pModel->m_pRecords->end())
			{
				for(int j=0;j<3;j++)
					m_pProductView[i]->m_pScope->Channels[j].Data.SetYData(
																(*it)->m_pOpticDepth->m_pData[j+1],
																pModel->m_pRecords->at(0)->m_pSamples->GetLength());
				it++;
			}
		}
	}
}

void __stdcall CAdvancedProductView::OnCustomDtAxesLabel(VCLHANDLE Sender, double Value, CString &label )
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	CString s;
	CTime t = m_tStart + Value * m_datetimeInterval;
	s = t.Format(_T("%Y/%m/%d-%H:%M:%S"));
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if(m_pProductView[i]->m_pWaterfall->XAxis.Get_VCL_Handle() == Sender)	
			{
				m_pProductView[i]->m_pWaterfall->XAxis.SetTickText(s);
				break;
			}
		}
		else if(m_pProductView[i]->m_pScope->XAxis.Get_VCL_Handle() == Sender)
		{
			m_pProductView[i]->m_pScope->XAxis.SetTickText(s);
			break;
		}
		else continue;	
	}
}

void __stdcall CAdvancedProductView::OnCustomHeightAxesLabel(VCLHANDLE Sender, double Value, CTString &label)
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CString axisLabel;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;
		if(m_pProductView[i]->m_ProductType != CProductHelper::ProductType_Cloud)
			continue;
		if(m_pProductView[i]->m_pWaterfall->YAxis.Get_VCL_Handle() != Sender)
			continue;

		axisLabel.Format(_T("%d"),(unsigned int)(Value * m_heightInterval));
		m_pProductView[i]->m_pWaterfall->YAxis.SetTickText(axisLabel);
		break;
	}
}

void __stdcall CAdvancedProductView::OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y)
{
	if(btn != mbRight)
		return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_AXISMENU));
	CMenu *pContextMenu = menu.GetSubMenu(0);
	/*CBitmap contextMenuBitmap;
	contextMenuBitmap.LoadBitmap(IDB_CONTEXTMENU);
	pContextMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&contextMenuBitmap,&contextMenuBitmap);*/

	CPoint pt;
	GetCursorPos(&pt);
	CString chName1(CProductHelper::CHANNEL_NAME[0]);
	CString chName2(CProductHelper::CHANNEL_NAME[1]);
	CString chName3(CProductHelper::CHANNEL_NAME[2]);

	int nMenus = pContextMenu->GetMenuItemCount();
	CString szText;
	for(int j=0;j<nMenus;j++)
	{
		pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
		if(szText == _T("色条") || szText == _T("X轴"))
		{
			pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			continue;
		}
	}

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;


		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{
				m_ContetxMenuSender = Sender;
				for(int j=0;j<nMenus;j++)
				{
					pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
					if(szText == chName1 || szText == chName2 || szText == chName3)
						pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
				}
				pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
				break;
			}
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			m_ContetxMenuSender = Sender;

			for(int j=0;j<nMenus;j++)
			{
				pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
				//FIXME:通道数固定
				if(szText == chName1)
				{
					if((m_pProductView[i]->m_pScope)->Channels[0].Visible)
						pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_CHECKED);
					else pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_UNCHECKED);
				}
				else if(szText == chName2)
				{
					if((m_pProductView[i]->m_pScope)->Channels[1].Visible)
						pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_CHECKED);
					else pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_UNCHECKED);
				}
				else if(szText == chName3)
				{
					if((m_pProductView[i]->m_pScope)->Channels[2].Visible)
						pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_CHECKED);
					else pContextMenu->CheckMenuItem(j,MF_BYPOSITION | MF_UNCHECKED);
				}
			}
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
			break;
		}	
	}
}

void __stdcall CAdvancedProductView::OnTSLMouseMove(VCLHANDLE Sender,CTShiftState shift,int x,int y)
{
	CString strToolTipText;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;
	
		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{	
				strToolTipText = m_pProductView[i]->m_pWaterfall->GetToolTipText();
				pMainFrame->UpdateCursorPos(strToolTipText);
				break;
			}
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			strToolTipText = m_pProductView[i]->m_pScope->GetToolTipText();
			pMainFrame->UpdateCursorPos(strToolTipText);
			break;
		}
	}
}

void __stdcall CAdvancedProductView::OnTSLDataToolTip(VCLHANDLE Sender,POINT &pt,CTSLRealPoint &realPt,CTString &tooltip)
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

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{	
				strToolTip.Format(_T("方位角度:270.90 俯仰角度:28.89 "));

				CString strDataVal;
				CString strDataFormat(CProductHelper::PRODUCT_NAME_FORMAT[m_pProductView[i]->m_ProductType]);
				strDataVal.Format(strDataFormat,realPt.Y * m_heightInterval);
				strToolTip = sTime + strToolTip;
				strToolTip += strDataVal;
				if(m_pProductView[i]->m_pWaterfall->DataView.GetValueAtScale(realPt.X,realPt.Y) < PRODUCT_CLOUD_COLOR[0]-10)
					strToolTip += _T(" 无云");

				m_pProductView[i]->m_pWaterfall->SetToolTipText(strToolTip);
				break;
			}
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			strToolTip.Format(_T("方位角度:270.90 俯仰角度:28.89 "));
			CString strDataVal;
			CString strDataFormat(CProductHelper::PRODUCT_NAME_FORMAT[m_pProductView[i]->m_ProductType]);
			strDataVal.Format(strDataFormat,realPt.Y);
			strToolTip = sTime + strToolTip;
			strToolTip += strDataVal;
			m_pProductView[i]->m_pScope->SetToolTipText(strToolTip);
			break;
		}

	}
}

void CAdvancedProductView::OnAxisconfigureCh1()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pScope)->Control.GetHandle() == m_ContetxMenuSender)
		{
			if(m_pProductView[i]->m_pScope->Channels[0].Visible)
				m_pProductView[i]->m_pScope->Channels[0].Visible = FALSE;
			else m_pProductView[i]->m_pScope->Channels[0].Visible = TRUE;
			break;
		}
	}
}

void CAdvancedProductView::OnAxisconfigureCh2()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pScope)->Control.GetHandle() == m_ContetxMenuSender)
		{
			if(m_pProductView[i]->m_pScope->Channels[1].Visible)
				m_pProductView[i]->m_pScope->Channels[1].Visible = FALSE;
			else m_pProductView[i]->m_pScope->Channels[1].Visible = TRUE;
			break;
		}
	}
}

void CAdvancedProductView::OnAxisconfigureCh3()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if((m_pProductView[i]->m_pScope)->Control.GetHandle() == m_ContetxMenuSender)
		{
			if(m_pProductView[i]->m_pScope->Channels[2].Visible)
				m_pProductView[i]->m_pScope->Channels[2].Visible = FALSE;
			else m_pProductView[i]->m_pScope->Channels[2].Visible = TRUE;
			break;
		}
	}
}

void CAdvancedProductView::ScrollProductView(CTime& tStart,CTime& tEnd)
{
	ClearViewData();
	m_tStart = tStart;
	/*for(unsigned int i=0;i<CProductMgr::MAX_PRODUCT_VIEW;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductMgr::ProductType_Cloud)
		{
			m_pProductView[i]->m_pWaterfall->XAxis.Min.DataValue = 0;
			m_pProductView[i]->m_pWaterfall->XAxis.Min.Value = 0;
		}
	}*/
}

void CAdvancedProductView::ScrollProductView()
{
	ClearViewData();
	m_tStart = CTime::GetCurrentTime();
	
	/*for(unsigned int i=0;i<CProductMgr::MAX_PRODUCT_VIEW;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		m_pProductView[i]->m_pWaterfall->XAxis.Min.DataValue = 0;
		m_pProductView[i]->m_pWaterfall->XAxis.Min.Value = 0;
	}*/
}

void CAdvancedProductView::ClearViewData()
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
			m_pProductView[i]->m_pWaterfall->Data.Clear();
		else 
		{
			for(int j=0;j<3;j++)
				m_pProductView[i]->m_pScope->Channels[j].Data.Clear();
		}
	}
}

void CAdvancedProductView::OnAxisconfigureVerticalaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == m_ContetxMenuSender)
			{
				dlg.m_Max = m_pProductView[i]->m_pWaterfall->YAxis.CurrentMax * m_heightInterval;
				dlg.m_Min = m_pProductView[i]->m_pWaterfall->YAxis.CurrentMin * m_heightInterval;
				dlg.m_Title = CProductHelper::PRODUCT_NAME[m_pProductView[i]->m_ProductType];
				dlg.m_bLogCheckEnable = FALSE;
				if(IDOK != dlg.DoModal())
					return;

				m_pProductView[i]->m_pWaterfall->YAxis.ZoomTo(dlg.m_Min / m_heightInterval,dlg.m_Max / m_heightInterval);
				m_pProductView[i]->m_pWaterfall->Hold = FALSE;
				return;
			}
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == m_ContetxMenuSender)
		{
			dlg.m_Max = m_pProductView[i]->m_pScope->YAxis.CurrentMax;
			dlg.m_Min = m_pProductView[i]->m_pScope->YAxis.CurrentMin;
			dlg.m_Title = CProductHelper::PRODUCT_NAME[m_pProductView[i]->m_ProductType];
			dlg.m_bLogCheckEnable = FALSE;
			if(IDOK != dlg.DoModal())
				return;

			m_pProductView[i]->m_pScope->YAxis.ZoomTo(dlg.m_Min,dlg.m_Max,FALSE);
			m_pProductView[i]->m_pScope->Hold = FALSE;
			return;
		}
	}
}
