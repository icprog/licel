// AdvancedProductView.cpp : 实现文件
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "AdvancedProductView.h"
#include "Radar_wspDoc.h"
#include "AxisSettingsDlg.h"
#include "MainFrm.h"

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
	ON_COMMAND(ID_AXISCONFIGURE_VERTICALAXIS, &CAdvancedProductView::OnAxisconfigureVerticalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_HORIZONTALAXIS, &CRadarFormView::OnAxisconfigureHorizontalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_LEVELAXIS, &CRadarFormView::OnAxisconfigureLevelaxis)
	ON_COMMAND(ID_AXISCONFIGURE_AXISALLRESET, &CRadarFormView::OnAxisconfigureAixsAllReset)
	ON_COMMAND(ID_AXISCONFIGURE_EXPORTIMG, &CRadarFormView::OnAxisconfigureExportImg)
	ON_COMMAND(ID_AXISCONFIGURE_PRINT, &CAdvancedProductView::OnAxisconfigurePrint)
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

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
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

	if(GetValidProductElementCount() == 0)
	{
		CPaintDC dc(this);
		dc.FillSolidRect(CRect(0,0,cx,cy),RGB(0,0,0));
		dc.SetTextColor(RGB(0,0,255));
		dc.DrawText(_T("请选择数据产品以供展示..."),14,
						CRect(0,0,cx,cy),DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		return;
	}

	int nDy = (cy-20) / GetValidProductElementCount();
	int y = 10;
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity)
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

	//软件启动后自动显示云高
	AddProductView(CProductHelper::ProductType_Cloud,CProductHelper::ProductChannelType_3ch);

	m_CurvesPerScreen = m_pProductView[0]->m_pWaterfall->XAxis.CurrentMax;

	m_bInitial = TRUE;
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
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	if(pDoc->m_ProductHelper.GetCount() != 0)
		pDoc->m_ProductHelper.Remove(pDoc->m_ProductHelper.GetCount() - 1);

	m_tStart = CTime::GetCurrentTime();
	
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OpticDepth
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			m_pProductView[i]->m_pWaterfall->YAxis.Max.DataValue = m_curvePointsSize;
			m_pProductView[i]->m_pWaterfall->YAxis.Max.Value = m_curvePointsSize;

			m_pProductView[i]->m_pWaterfall->Invalidate();
		}
		else 
		{
			m_pProductView[i]->m_pScope->XAxis.Max.DataValue = m_CurvesPerScreen;
			m_pProductView[i]->m_pScope->XAxis.Max.Value = m_CurvesPerScreen;

			m_pProductView[i]->m_pScope->Invalidate();
		}
	}
}

BOOL CAdvancedProductView::AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	BOOL bReLayout = FALSE;

	unsigned int nIdx = GetIdealProductElement();
	if(nIdx == -1)
		return FALSE;

	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisConfigHelper::PTR_AxisSettings pAxisSettings = pDoc->m_AxisConfigHelper.GetProductConfig(product,ch);

	m_pProductView[nIdx] = new ProductView;
	m_pProductView[nIdx]->m_ProductType = product;
	m_pProductView[nIdx]->m_Ch = ch;	
	m_pProductView[nIdx]->m_pStatic[0] = new CStatic();
	m_pProductView[nIdx]->m_pStatic[0]->Create(_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,300,300),this,0);

	CString title(CProductHelper::PRODUCT_NAME[product]);
	CString yAxisLabel(CProductHelper::PRODUCT_NAME_UNIT[product]);
	if(m_pProductView[nIdx]->m_ProductType == CProductHelper::ProductType_Cloud
		|| m_pProductView[nIdx]->m_ProductType == CProductHelper::ProductType_Visiblity
		|| m_pProductView[nIdx]->m_ProductType == CProductHelper::ProductType_MassConcentration)
	{
		m_pProductView[nIdx]->m_pWaterfall = new CTSLWaterfall();
		m_pProductView[nIdx]->m_pWaterfall->Open(m_pProductView[nIdx]->m_pStatic[0]->m_hWnd);
		m_pProductView[nIdx]->m_pWaterfall->Align = CTAlign::alClient;
		m_pProductView[nIdx]->m_pWaterfall->ToolBar.Visible = FALSE;
		m_pProductView[nIdx]->m_pWaterfall->Title.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pWaterfall->Title.Font.Size = 11;
		CString strTemp(CProductHelper::CHANNEL_NAME[ch]);
		title += _T("-");
		title += strTemp;
		m_pProductView[nIdx]->m_pWaterfall->Title.Text = title;
		m_pProductView[nIdx]->m_pWaterfall->Title.Align = vaLeft;
	
		m_pProductView[nIdx]->m_pWaterfall->OnMouseMove.Set(this,&CAdvancedProductView::OnTSLMouseMove);
		m_pProductView[nIdx]->m_pWaterfall->OnMouseUp.Set(this,&CAdvancedProductView::OnTSLMouseUp);//弹出上下文菜单
		m_pProductView[nIdx]->m_pWaterfall->OnDataToolTip.Set(this,&CAdvancedProductView::OnTSLDataToolTip);//自定义ToolTip
	
		if(m_pProductView[nIdx]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			m_pProductView[nIdx]->m_pWaterfall->Levels.Visible = FALSE;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Add(9);
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Autoscale = FALSE;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Max = 1;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Min = 0;
		}
		else
		{
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Autoscale = FALSE;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Max = pAxisSettings->level_max;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.Min = pAxisSettings->level_min;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Axis.ScaleMode = pAxisSettings->log_c ? smLogarithmic : smLinear;
			m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Add(5);
			m_pProductView[nIdx]->m_pWaterfall->Levels.Colors.Delete(0);
			m_pProductView[nIdx]->m_pWaterfall->Levels.Visible = pAxisSettings->level_visible;
		}

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
		m_pProductView[nIdx]->m_pScope->Title.Font.Size = 11;
		m_pProductView[nIdx]->m_pScope->Title.Text = title;
		m_pProductView[nIdx]->m_pScope->Title.Align = vaLeft;
		m_pProductView[nIdx]->m_pScope->OnMouseMove.Set(this,&CAdvancedProductView::OnTSLMouseMove);
		m_pProductView[nIdx]->m_pScope->OnMouseUp.Set(this,&CAdvancedProductView::OnTSLMouseUp);//弹出上下文菜单
		m_pProductView[nIdx]->m_pScope->OnDataToolTip.Set(this,&CAdvancedProductView::OnTSLDataToolTip);//自定义ToolTip

		m_pProductView[nIdx]->m_pScope->XAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.Min.DataValue = 0;
		m_pProductView[nIdx]->m_pScope->XAxis.Min.Value = 0;
		m_pProductView[nIdx]->m_pScope->XAxis.Max.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.Max.DataValue = m_CurvesPerScreen;//FIXME:
		m_pProductView[nIdx]->m_pScope->XAxis.Max.Value = m_CurvesPerScreen;
		m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pScope->XAxis.DataView.Lines.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.DataView.ZeroLine.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->XAxis.AxisLabel.Text = _T("时间");
		m_pProductView[nIdx]->m_pScope->XAxis.OnCustomLabel.Set(this,&CAdvancedProductView::OnCustomDtAxesLabel);//自定义X轴显示日期时间

		m_pProductView[nIdx]->m_pScope->YAxis.Min.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.Min.DataValue = pAxisSettings->default_min;
		m_pProductView[nIdx]->m_pScope->YAxis.Min.Value = pAxisSettings->default_min;
		m_pProductView[nIdx]->m_pScope->YAxis.Max.AutoScale = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.Max.DataValue = pAxisSettings->default_max;
		m_pProductView[nIdx]->m_pScope->YAxis.Max.Value = pAxisSettings->default_max;
		m_pProductView[nIdx]->m_pScope->YAxis.ScaleMode = pAxisSettings->log_t ? smLogarithmic : smLinear;
		m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Font.Charset = GB2312_CHARSET;
		m_pProductView[nIdx]->m_pScope->YAxis.DataView.Lines.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.DataView.ZeroLine.Visible = FALSE;
		m_pProductView[nIdx]->m_pScope->YAxis.AxisLabel.Text = yAxisLabel;
	
		m_pProductView[nIdx]->m_pScope->Channels.Add();
		m_pProductView[nIdx]->m_pScope->Channels.Add();
		CString chName1(CProductHelper::CHANNEL_NAME[CProductHelper::ProductChannelType_1ch]);
		CString chName2(CProductHelper::CHANNEL_NAME[CProductHelper::ProductChannelType_2ch]);
		CString chName3(CProductHelper::CHANNEL_NAME[CProductHelper::ProductChannelType_3ch]);
		m_pProductView[nIdx]->m_pScope->Channels[CProductHelper::ProductChannelType_1ch].Name = chName1;
		m_pProductView[nIdx]->m_pScope->Channels[CProductHelper::ProductChannelType_2ch].Name = chName2;
		m_pProductView[nIdx]->m_pScope->Channels[CProductHelper::ProductChannelType_3ch].Name = chName3;
	}
	
	VCL_Loaded();
	ReLayoutProductView();

	return TRUE;
}

void CAdvancedProductView::StartRealTimeView(CAcquireHelper* pAcquireHelper)
{
	/*m_heightInterval = pAcquireHelper->GetHeightScaleFactor();
	m_datetimeInterval = pAcquireHelper->GetDtScaleFactor();
	m_curvePointsSize = pAcquireHelper->GetCurvePointCount();*/
	CProductBaseView::StartRealTimeView(pAcquireHelper);
	ScrollProductView();
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
						pCloud[j] = 1;
					else pCloud[j] = 0;
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

void CAdvancedProductView::UpdateView(CDatModel* pModel,CProductHelper::ProductType product,CProductHelper::ProductChannelType ch)
{
	ASSERT(pModel);

	unsigned int sum = pModel->m_pRecords->size();
	if(sum <= 1)
		return;

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType != product)
			continue;

		vector<CDatRecord* >::iterator it = pModel->m_pRecords->begin();

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			while(it != pModel->m_pRecords->end() - 1)
			{
				m_pProductView[i]->m_pWaterfall->Data.AddData(
											(*it)->m_pCloud->m_pData[1],
											(*it)->m_pCloud->GetLength());				
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OpticDepth)
		{
			while(it != pModel->m_pRecords->end() - 1)
			{
				for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
					m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint((*it)->m_pOpticDepth->m_pData[j+1][0]);
				
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_AtmosphereBoundary)
		{
			while(it != pModel->m_pRecords->end() - 1)
			{
				for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
					m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint((*it)->m_pBoundary->m_pData[j+1][0]);
				
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity)
		{
			if(m_pProductView[i]->m_Ch != ch)
				continue;

			while(it != pModel->m_pRecords->end() - 1)
			{	
				m_pProductView[i]->m_pWaterfall->Data.AddData((*it)->m_pVisiblity->m_pData[m_pProductView[i]->m_Ch+1],
																(*it)->m_pVisiblity->GetLength());
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MixHeight)
		{
			while(it != pModel->m_pRecords->end() - 1)
			{
				for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
					m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint((*it)->m_pMix->m_pData[j+1][0]);
				
				it++;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			while(it != pModel->m_pRecords->end() - 1)
			{
				m_pProductView[i]->m_pWaterfall->Data.AddData(
											(*it)->m_pGasoloidPotency->m_pData[1],
											(*it)->m_pGasoloidPotency->GetLength());				
				it++;
			}
		}
	}
}

void CAdvancedProductView::UpdateView(CDatRecord* pRecord)
{
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			m_pProductView[i]->m_pWaterfall->Data.AddData(
										pRecord->m_pCloud->m_pData[1],
										pRecord->m_pCloud->GetLength());
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_OpticDepth)
		{
			for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
				m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint(pRecord->m_pOpticDepth->m_pData[j+1][0]);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_AtmosphereBoundary)
		{
			for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
				m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint(pRecord->m_pBoundary->m_pData[j+1][0]);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity)
		{
			m_pProductView[i]->m_pWaterfall->Data.AddData(
										pRecord->m_pVisiblity->m_pData[m_pProductView[i]->m_Ch+1],
										pRecord->m_pVisiblity->GetLength());
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MixHeight)
		{
			for(int j=0;j<CProductHelper::ProductChannelType_Sum;j++)
				m_pProductView[i]->m_pScope->Channels[j].Data.AddYPoint(pRecord->m_pMix->m_pData[j+1][0]);
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			m_pProductView[i]->m_pWaterfall->Data.AddData(
										pRecord->m_pGasoloidPotency->m_pData[1],
										pRecord->m_pGasoloidPotency->GetLength());
		}
	}
}

void __stdcall CAdvancedProductView::OnCustomDtAxesLabel(VCLHANDLE Sender, double Value, CString &label )
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	
	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;
		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			CString s;
			CTime t = m_tStart + Value * m_datetimeInterval;
			s = t.Format(_T("%Y/%m/%d-%H:%M:%S"));

			if(m_pProductView[i]->m_pWaterfall->XAxis.Get_VCL_Handle() == Sender)	
			{
				m_pProductView[i]->m_pWaterfall->XAxis.SetTickText(s);
				break;
			}
		}
		else if(m_pProductView[i]->m_pScope->XAxis.Get_VCL_Handle() == Sender)
		{
			CString s;
			CTime t = m_tStart + Value * m_datetimeInterval;
			s = t.Format(_T("%Y/%m/%d-%H:%M:%S"));
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
		if(m_pProductView[i]->m_ProductType != CProductHelper::ProductType_Cloud
			&& m_pProductView[i]->m_ProductType != CProductHelper::ProductType_Visiblity
			&& m_pProductView[i]->m_ProductType != CProductHelper::ProductType_MassConcentration)
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

	int nMenus = pContextMenu->GetMenuItemCount();
	CString szText;

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{
				m_pSelectedWaterfall = m_pProductView[i]->m_pWaterfall;
				m_pSelectedScope = NULL;
				m_SelectProductType = m_pProductView[i]->m_ProductType;
				m_SelectChannelType = m_pProductView[i]->m_Ch;

				for(int j=0;j<nMenus;j++)
				{
					pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
					if(szText == _T("色条") || szText == _T("X轴"))
						pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
				}
				pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
				break;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{
				m_pSelectedWaterfall = m_pProductView[i]->m_pWaterfall;
				m_pSelectedScope = NULL;
				m_SelectProductType = m_pProductView[i]->m_ProductType;
				m_SelectChannelType = m_pProductView[i]->m_Ch;

				for(int j=0;j<nMenus;j++)
				{
					pContextMenu->GetMenuStringW(j,szText,MF_BYPOSITION);
					if(szText == _T("X轴"))
						pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
				}
				pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
				break;
			}
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
				if(szText == _T("色条") || szText == _T("X轴"))
					pContextMenu->EnableMenuItem(j,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
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
	
		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
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

	/*搜索时间对应的记录*/
	CProductHelper *pProductHelper = &(pDoc->m_ProductHelper);
	CDatRecord* pRecord = pProductHelper->SearchRecord(t);
	double fy = 0,fw = 0;
	if(pRecord != NULL)
	{
		fy = pRecord->m_FyAngle;
		fw = pRecord->m_FwAngle;
	}

	for(unsigned int i=0;i<MAX_PRODUCT_VIEW_COUNT;i++)
	{
		if(m_pProductView[i] == NULL)
			continue;

		if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Cloud)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{	
				strToolTip.Format(_T("方位角度:%.2f 俯仰角度:%.2f 高度:%.0f "),
								fw,
								fy,
								realPt.Y * m_heightInterval);
				CString strDataVal;
				CString strDataFormat(CProductHelper::PRODUCT_NAME_FORMAT[m_pProductView[i]->m_ProductType]);
				strDataVal.Format(strDataFormat,realPt.Y * m_heightInterval);
				strToolTip = sTime + strToolTip;
				strToolTip += strDataVal;
				if(m_pProductView[i]->m_pWaterfall->DataView.GetValueAtScale(realPt.X,realPt.Y) < 0.5)
					strToolTip += _T(" 无云");
				else strToolTip += _T(" 有云");

				m_pProductView[i]->m_pWaterfall->SetToolTipText(strToolTip);
				break;
			}
		}
		else if(m_pProductView[i]->m_ProductType == CProductHelper::ProductType_Visiblity
			|| m_pProductView[i]->m_ProductType == CProductHelper::ProductType_MassConcentration)
		{
			if((m_pProductView[i]->m_pWaterfall)->Control.GetHandle() == Sender)
			{	
				strToolTip.Format(_T("方位角度:%.2f 俯仰角度:%.2f 高度:%.0f "),
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
		}
		else if((m_pProductView[i]->m_pScope)->Control.GetHandle() == Sender)
		{
			strToolTip.Format(_T("方位角度:%.2f 俯仰角度:%.2f "),
								fw,
								fy);
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

void CAdvancedProductView::OnAxisconfigureHorizontalaxis()
{
	/*CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();

	CAxisSettingsDlg dlg; 
	if(m_pSelectedScope == NULL)
		return;

	dlg.m_Max = m_pSelectedScope->XAxis.CurrentMax;
	dlg.m_Min = m_pSelectedScope->XAxis.CurrentMin;
	dlg.m_Title = _T("水平坐标轴设置");
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
	m_pSelectedScope->Hold = FALSE;*/
}

void CAdvancedProductView::OnAxisconfigureVerticalaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;

	if((m_SelectProductType == CProductHelper::ProductType_Cloud
		|| m_SelectProductType == CProductHelper::ProductType_Visiblity)
		&& m_pSelectedWaterfall != NULL)
	{
		dlg.m_Max = m_pSelectedWaterfall->YAxis.CurrentMax * m_heightInterval;
		dlg.m_Min = m_pSelectedWaterfall->YAxis.CurrentMin * m_heightInterval;
		dlg.m_Title = _T("垂直坐标轴设置(高度/m)");
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
		dlg.m_Title = _T("水平坐标轴设置");
		dlg.m_bLogCheckEnable = TRUE;
		dlg.m_bLogEnable = (m_pSelectedScope->YAxis.ScaleMode == smLogarithmic);
		if(IDOK != dlg.DoModal())
			return;

		CAxisConfigHelper::PTR_AxisSettings pAxisSettings = pDoc->m_AxisConfigHelper.GetProductConfig(m_SelectProductType,m_SelectChannelType);
		pAxisSettings->log_t = dlg.m_bLogEnable;
		pAxisSettings->min = dlg.m_Min;
		pAxisSettings->max = dlg.m_Max;
		pDoc->m_AxisConfigHelper.SaveModify(m_SelectProductType,m_SelectChannelType);

		m_pSelectedScope->YAxis.ScaleMode = pAxisSettings->log_t ? smLogarithmic : smLinear;
		m_pSelectedScope->YAxis.ZoomTo(pAxisSettings->min,pAxisSettings->max,FALSE);
		m_pSelectedScope->Hold = FALSE;
	}
}

void CAdvancedProductView::OnAxisconfigureLevelaxis()
{
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)GetDocument();
	CAxisSettingsDlg dlg;
	if(m_pSelectedWaterfall == NULL)
		return;

	dlg.m_Max = m_pSelectedWaterfall->Levels.Axis.Max;
	dlg.m_Min = m_pSelectedWaterfall->Levels.Axis.Min;
	dlg.m_Title = _T("色条坐标轴设置");
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

void CAdvancedProductView::OnAxisconfigureAixsAllReset()
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

void CAdvancedProductView::OnAxisconfigureExportImg()
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
		m_pSelectedWaterfall->SaveToBitmap(f.GetPathName());
	}
	else return;	
}

void CAdvancedProductView::OnAxisconfigurePrint()
{
	if(m_pSelectedWaterfall != NULL)
	{
		m_pSelectedWaterfall->Print();
	}
	else if(m_pSelectedScope != NULL)
	{
		m_pSelectedWaterfall->Print();
	}
	else return;
}
