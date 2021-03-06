// DataBrowerDlg.cpp
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "DataBrowserDlg.h"
#include "afxdialogex.h"
#include "AxisSettingsDlg.h"
#include "RadarFormView.h"

static UINT BASED_CODE indicators[]=  
{
	ID_SEPARATOR,
    ID_INDICATOR_SEARCHRESULT,
	ID_INDICATOR_TASKPROGRESS,
};


// CDataBrowerDlg 对话框

IMPLEMENT_DYNAMIC(CDataBrowserDlg, CDialogEx)

CDataBrowserDlg::CDataBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataBrowserDlg::IDD, pParent)
{
	m_pDatRecords = NULL;
}

CDataBrowserDlg::~CDataBrowserDlg()
{
}

void CDataBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_BrowserEdit);
	DDX_Control(pDX, IDC_RECORDLIST, m_RecordList);
	DDX_Control(pDX, IDC_DATESTARTPICKER, m_DateStartPicker);
	DDX_Control(pDX, IDC_TIMESTARTPICKER, m_TimeStartPicker);
	DDX_Control(pDX, IDC_DATEENDPICKER, m_DateEndPicker);
	DDX_Control(pDX, IDC_TIMEENDPICKER, m_TimeEndPicker);

	DDX_Control(pDX, IDC_BTN_SIXHOUR, m_BtnSixHour);
	DDX_Control(pDX, IDC_BTN_HOUR, m_BtnHour);
	DDX_Control(pDX, IDC_BTN_HALFDAY, m_BtnHalfDay);
	DDX_Control(pDX, IDC_BTN_DAY, m_BtnDay);
	DDX_Control(pDX, IDC_COMBO_STATION, m_ComboStation);
	DDX_Control(pDX, IDC_COMBO_CONDUCTOR2, m_ComboConductor);
	DDX_Control(pDX, IDC_BTN_SEARCHRESULT, m_BtnSearchResult);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_ComboPeriod);
}

BEGIN_MESSAGE_MAP(CDataBrowserDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCH, &CDataBrowserDlg::OnBnClickedSearch)
	ON_COMMAND(ID_AXISCONFIGURE_HORIZONTALAXIS, &CDataBrowserDlg::OnAxisconfigureHorizontalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_VERTICALAXIS, &CDataBrowserDlg::OnAxisconfigureVerticalaxis)
	ON_COMMAND(ID_AXISCONFIGURE_AXISALLRESET, &CDataBrowserDlg::OnAxisconfigureAixsAllReset)
	ON_NOTIFY(NM_CLICK, IDC_RECORDLIST, &CDataBrowserDlg::OnNMClickRecordlist)
	ON_BN_CLICKED(IDOK, &CDataBrowserDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDataBrowerDlg 消息处理程序


BOOL CDataBrowserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wndStatusBar.CreateEx(this,SBT_TOOLTIPS,WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,AFX_IDW_STATUS_BAR);
	m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));//设置指示器数量
	CRect rect;
    GetClientRect(&rect);
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_SEARCHRESULT,SBPS_NORMAL,100);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_TASKPROGRESS,SBPS_STRETCH ,100);
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_SEPARATOR);
    m_wndStatusBar.GetStatusBarCtrl().SetBkColor(RGB(180,20,180));

	m_wndStatusBar.GetItemRect(2,&rect);
	m_ProgressCtrl.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,rect,&m_wndStatusBar,0);
	m_ProgressCtrl.SetRange(0, 100);  
	m_ProgressCtrl.SetPos(0); 

	m_BtnSearchResult.EnableMarkup();
	m_BtnSearchResult.SetWindowText(_T("<TextBlock><Bold><Hyperlink>在结果中搜索</Hyperlink></Bold></TextBlock>"));
	m_BtnSearchResult.SetFlatStyle(TRUE);
	m_BtnSearchResult.GetMarkupContext()->AddHandler(CXTPMarkupHyperlink::m_pClickEvent,
											CreateMarkupClassDelegate(this,&CDataBrowserDlg::OnBnClickedBtnSearchresult));

	m_BrowserEdit.Initialize(this, BES_XTP_CHOOSEDIR);

	m_BtnSixHour.EnableMarkup();
	m_BtnSixHour.SetWindowText(_T("<TextBlock><Bold><Hyperlink>6小时</Hyperlink></Bold></TextBlock>"));
	m_BtnSixHour.SetFlatStyle(TRUE);
	m_BtnSixHour.GetMarkupContext()->AddHandler(CXTPMarkupHyperlink::m_pClickEvent,
											CreateMarkupClassDelegate(this,&CDataBrowserDlg::OnBnClickedBtnSixhour));

	m_BtnHour.EnableMarkup();
	m_BtnHour.SetWindowText(_T("<TextBlock><Bold><Hyperlink>一小时</Hyperlink></Bold></TextBlock>"));
	m_BtnHour.SetFlatStyle(TRUE);
	m_BtnHour.GetMarkupContext()->AddHandler(CXTPMarkupHyperlink::m_pClickEvent,
											CreateMarkupClassDelegate(this,&CDataBrowserDlg::OnBnClickedBtnHour));

	m_BtnHalfDay.EnableMarkup();
	m_BtnHalfDay.SetWindowText(_T("<TextBlock><Bold><Hyperlink>12小时</Hyperlink></Bold></TextBlock>"));
	m_BtnHalfDay.SetFlatStyle(TRUE);
	m_BtnHalfDay.GetMarkupContext()->AddHandler(CXTPMarkupHyperlink::m_pClickEvent,
											CreateMarkupClassDelegate(this,&CDataBrowserDlg::OnBnClickedBtnHalfday));

	m_BtnDay.EnableMarkup();
	m_BtnDay.SetWindowText(_T("<TextBlock><Bold><Hyperlink>24小时</Hyperlink></Bold></TextBlock>"));
	m_BtnDay.SetFlatStyle(TRUE);
	m_BtnDay.GetMarkupContext()->AddHandler(CXTPMarkupHyperlink::m_pClickEvent,
											CreateMarkupClassDelegate(this,&CDataBrowserDlg::OnBnClickedBtnDay));

	m_ComboStation.AddString(_T("*"));
	m_ComboConductor.AddString(_T("*"));
	m_ComboStation.SetCurSel(0);
	m_ComboConductor.SetCurSel(0);

	m_RecordList.InsertColumn(0,_T("时间"),0,180);
	m_RecordList.InsertColumn(1,_T("脉冲频率(MHz)"),0,110);
	m_RecordList.InsertColumn(2,_T("通道数"),0,70);
	m_RecordList.InsertColumn(3,_T("脉冲数"),0,70);
	m_RecordList.InsertColumn(4,_T("采样间隔(s)"),0,80);
	m_RecordList.InsertColumn(5,_T("站点"),0,100);
	m_RecordList.InsertColumn(6,_T("操作者"),0,100);
	m_RecordList.InsertColumn(7,_T("备注"),0,100);

	m_Scope.Open(GetDlgItem(IDC_PLOTSTATIC)->GetSafeHwnd());
	m_Scope.Align = CTAlign::alClient;
	m_Scope.ToolBar.Visible = FALSE;
	m_Scope.Title.Visible = FALSE;
	m_Scope.Legend.Visible = TRUE;
	m_Scope.OnMouseUp.Set(this,&CDataBrowserDlg::OnTSLMouseUp);

	m_Scope.Channels.Add();
	m_Scope.Channels.Add();
	CString chName1(CProductHelper::CHANNEL_NAME[0]);
	CString chName2(CProductHelper::CHANNEL_NAME[1]);
	CString chName3(CProductHelper::CHANNEL_NAME[2]);
	m_Scope.Channels[0].Name = chName1;
	m_Scope.Channels[1].Name = chName2;
	m_Scope.Channels[2].Name = chName3;

	m_Scope.XAxis.Min.AutoScale = FALSE;
	m_Scope.XAxis.Min.DataValue = 0;
	m_Scope.XAxis.Min.Value = 0;
	m_Scope.XAxis.Max.AutoScale = FALSE;
	m_Scope.XAxis.Max.DataValue = 42767;
	m_Scope.XAxis.Max.Value = 42767;
	m_Scope.XAxis.DataView.Lines.Visible = FALSE;
	m_Scope.XAxis.DataView.ZeroLine.Visible = FALSE;
	m_Scope.XAxis.AxisLabel.Text = _T("原始信号");

	m_Scope.YAxis.Min.AutoScale = FALSE;
	m_Scope.YAxis.Min.DataValue = 0;
	m_Scope.YAxis.Min.Value = 0;
	m_Scope.YAxis.Max.AutoScale = FALSE;
	m_Scope.YAxis.Max.DataValue = 12000.0;
	m_Scope.YAxis.Max.Value = 12000.0;
	m_Scope.YAxis.DataView.Lines.Visible = FALSE;
	m_Scope.YAxis.DataView.ZeroLine.Visible = FALSE;
	m_Scope.YAxis.AxisLabel.Text = _T("高度(m)");

	VCL_Loaded();

	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\browser.ini");
	unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("localfiles"),_T("dir"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	CString keyString;
	keyString.Append((LPWSTR)cBuf);
	m_BrowserEdit.SetWindowText(keyString);

	unsigned int nYear,nMonth,nDay,nHour,nMin,nSec;
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("filter"),_T("tStart"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	swscanf((LPWSTR)cBuf, _T("%0004d/%02d/%02d-%02d%02d%02d"), &nYear,&nMonth,&nDay,&nHour, &nMin, &nSec); 
	CTime tStart(nYear,nMonth,nDay,nHour,nMin,nSec);

	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("filter"),_T("tEnd"),_T(""),(LPWSTR)cBuf,_MAX_PATH,iniFile);
	swscanf((LPWSTR)cBuf, _T("%0004d/%02d/%02d-%02d%02d%02d"), &nYear,&nMonth,&nDay,&nHour, &nMin, &nSec); 
	CTime tEnd(nYear,nMonth,nDay,nHour,nMin,nSec);

	SetTimeRange(tStart,tEnd);

	int period = GetPrivateProfileInt(_T("filter"),_T("period"),10,iniFile);
	for(int i=0;i<CAcquireHelper::AcquirePeriod_Sum;i++)
	{
		CString caption(CAcquireHelper::AcquirePeriod_Caption[i]);
		int idx = m_ComboPeriod.AddString(caption);
		if(period == CAcquireHelper::AcquirePeriod_Value[i])
			m_ComboPeriod.SetCurSel(idx);
	}
	
	return TRUE;
}

void CDataBrowserDlg::OnBnClickedSearch()
{
	ClearRecordList();

	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\browser.ini");

	CTime tDStart,tDEnd,tTStart,tTEnd;
	m_DateStartPicker.GetTime(tDStart);
	m_TimeStartPicker.GetTime(tTStart);
	m_DateEndPicker.GetTime(tDEnd);
	m_TimeEndPicker.GetTime(tTEnd);

	CTime tStart(tDStart.GetYear(),tDStart.GetMonth(),tDStart.GetDay(),tTStart.GetHour(),tTStart.GetMinute(),tTStart.GetSecond());
	CTime tEnd(tDEnd.GetYear(),tDEnd.GetMonth(),tDEnd.GetDay(),tTEnd.GetHour(),tTEnd.GetMinute(),tTEnd.GetSecond());
	if(tEnd < tStart)
	{
		MessageBox(_T("开始时间应早于结束时间!"),_T("错误"),MB_ICONERROR);
		return;
	}

	CString tempString;
	tempString = tStart.Format(_T("%Y/%m/%d-%H%M%S"));
	WritePrivateProfileString(_T("filter"),_T("tStart"),tempString,iniFile);
	tempString = tEnd.Format(_T("%Y/%m/%d-%H%M%S"));
	WritePrivateProfileString(_T("filter"),_T("tEnd"),tempString,iniFile);

	CString path;
	m_BrowserEdit.GetWindowText(path);
	WritePrivateProfileString(_T("localfiles"),_T("dir"),path,iniFile);

	unsigned int period = CAcquireHelper::AcquirePeriod_Value[m_ComboPeriod.GetCurSel()];

	QueryFolder(path,tStart,tEnd,period);
}

void CDataBrowserDlg::QueryFolder(CString folder,CTime& tStart,CTime& tEnd,unsigned int period)
{
	CFileFind ff;
	CString strTemp;
	CStringA strFileName;
	CTime tCurrent = tStart;

	CDatRecord* pRecord;
	unsigned int idx = 0;

	CString station,conductor;

	m_ComboStation.GetLBText(m_ComboStation.GetCurSel(),station);
	m_ComboConductor.GetLBText(m_ComboConductor.GetCurSel(),conductor);
	m_ComboStation.ResetContent();
	m_ComboConductor.ResetContent();

	BeginWaitCursor();

	do
	{
		strTemp.Format(_T("\\%d年\\%02d月\\%02d日\\*.rads"),tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay());
		CString path = folder + strTemp;
		if(!ff.FindFile(path))
			continue;

		CStringA fileName;
		unsigned int nHour,nMin,nSec;
		while(ff.FindNextFile())
		{
			if(ff.IsDots() || ff.IsDirectory())
				continue;
			
			pRecord = new CDatRecord();

			strFileName = ff.GetFileTitle();
			sscanf(strFileName, "%02d%02d%02d",&nHour, &nMin, &nSec); 

			CTime t(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(),nHour,nMin,nSec);
			pRecord->m_Time = t;

			/*if(!pRecord->LoadItemHeader(ff.GetFilePath()))
			{
				delete pRecord;
				continue;
			}*/

			if(pRecord->m_FramePeriod == period)
			{
				if(Record2List(pRecord,station,conductor))
					idx++;
			}
			else 
			{
				delete pRecord;
				continue;
			}

			m_ProgressCtrl.SetPos(idx % 100); 
		}
	}
	while((tCurrent = tCurrent + CTimeSpan(1,0,0,0)) <= tEnd);

	EndWaitCursor();

	UpdateFilters();

	strTemp.Format(_T("共%d个结果"),idx);
	m_wndStatusBar.SetPaneText(1,strTemp);
	m_ProgressCtrl.SetPos(0); 
}

void CDataBrowserDlg::QueryResult(CTime& tStart,CTime& tEnd,unsigned int period)
{
	unsigned int sum = m_RecordList.GetItemCount();
	if(sum == 0)
		return;

	CDatRecord* pRecord;

	CString strTemp,station,conductor;

	m_ComboStation.GetLBText(m_ComboStation.GetCurSel(),station);
	m_ComboConductor.GetLBText(m_ComboConductor.GetCurSel(),conductor);

	BeginWaitCursor();
	for(unsigned int i=0;i<sum;i++)
	{
		pRecord = (CDatRecord *)(m_RecordList.GetItemData(i));
		if(pRecord->m_Time < tStart || pRecord->m_Time > tEnd)
		{
			pRecord->Clear();
			m_RecordList.DeleteItem(i);
			i--;
			sum--;
			continue;
		}

		if(period != pRecord->m_FramePeriod)
		{
			if(period != 0)
			{
				pRecord->Clear();
				m_RecordList.DeleteItem(i);
				i--;
				sum--;
				continue;
			}
		}

		if(station != pRecord->m_Station)
		{
			if(station != _T("*"))
			{			
				pRecord->Clear();
				m_RecordList.DeleteItem(i);
				i--;
				sum--;
				continue;
			}
		}

		if(conductor != pRecord->m_Conductor)
		{
			if(conductor != _T("*"))
			{			
				pRecord->Clear();
				m_RecordList.DeleteItem(i);
				i--;
				sum--;
				continue;
			}
		}
	}
}

BOOL CDataBrowserDlg::Record2List(CDatRecord *pRecord,CString station,CString conductor)
{
	if(station != pRecord->m_Station)
	{
		if(station != _T("*"))
			return FALSE;
	}

	if(conductor != pRecord->m_Conductor)
	{
		if(conductor != _T("*"))
			return FALSE;
	}

	int count = m_RecordList.GetItemCount();
	CString strTemp;
	strTemp = pRecord->m_Time.Format(_T("%Y年\\%m月\\%d日\\%H%M%S"));
	count = m_RecordList.InsertItem(count,strTemp,0);

	CString folder;
	m_BrowserEdit.GetWindowText(folder);
	folder =  folder + _T("\\") + strTemp + _T(".rads");
	//pRecord->m_FilePath = folder;

	strTemp.Format(_T("%.3f"),pRecord->m_SampleFreq/1000000); 
	m_RecordList.SetItemText(count,1,strTemp);
	strTemp.Format(_T("%d"),pRecord->m_Chs); 
	m_RecordList.SetItemText(count,2,strTemp);
	strTemp.Format(_T("%d"),pRecord->m_SampleNum); 
	m_RecordList.SetItemText(count,3,strTemp);
	strTemp.Format(_T("%d"),pRecord->m_FramePeriod); 
	m_RecordList.SetItemText(count,4,strTemp);
	m_RecordList.SetItemText(count,5,pRecord->m_Station);
	m_RecordList.SetItemText(count,6,pRecord->m_Conductor);
	m_RecordList.SetItemText(count,7,pRecord->m_Comment);

	m_RecordList.SetItemData(count,(DWORD_PTR)pRecord);

	m_StationList.insert(pRecord->m_Station);
	m_ConductorList.insert(pRecord->m_Conductor);

	return TRUE;
}

void CDataBrowserDlg::ClearRecordList()
{
	int sum = m_RecordList.GetItemCount();
	CDatRecord *pRecord;
	for(int i=0;i<sum;i++)
	{
		pRecord = (CDatRecord *)m_RecordList.GetItemData(i);
		pRecord->Clear();
	}
	m_RecordList.DeleteAllItems();
}

void CDataBrowserDlg::PlotDat(CDatRecord &record)
{
	m_Scope.Channels[0].Data.Clear();
	m_Scope.Channels[0].Data.SetXYData(record.m_pSamples->m_pData[1],record.m_pSamples->m_pData[0],record.m_pSamples->GetLength());
	m_Scope.Channels[1].Data.Clear();
	m_Scope.Channels[1].Data.SetXYData(record.m_pSamples->m_pData[2],record.m_pSamples->m_pData[0],record.m_pSamples->GetLength());
	m_Scope.Channels[2].Data.Clear();
	m_Scope.Channels[2].Data.SetXYData(record.m_pSamples->m_pData[3],record.m_pSamples->m_pData[0],record.m_pSamples->GetLength());
}

void __stdcall CDataBrowserDlg::OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y)
{
	if(btn != mbRight)
		return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_POPUPMENU));
	CMenu *pContextMenu = menu.GetSubMenu(0);

	//禁止色条和复位
	int nMenus = pContextMenu->GetMenuItemCount();
	CString szText;
	for(int i=0;i<nMenus;i++)
	{
		pContextMenu->GetMenuStringW(i,szText,MF_BYPOSITION);
		if(szText == _T("色条") /*|| szText == _T("复位")*/)
			pContextMenu->EnableMenuItem(i,MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	

	CPoint pt;
	GetCursorPos(&pt);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
}

void CDataBrowserDlg::OnAxisconfigureHorizontalaxis()
{
	CAxisSettingsDlg dlg;
	
	dlg.m_Max = m_Scope.XAxis.CurrentMax;
	dlg.m_Min = m_Scope.XAxis.CurrentMin;
	dlg.m_Title = _T("Axis-X(Height/m)");
	dlg.m_bLogCheckEnable = TRUE;
	dlg.m_bLogEnable = (m_Scope.XAxis.ScaleMode == smLogarithmic);
	if(IDOK != dlg.DoModal())
		return;

	//if(dlg.m_Max > m_Scope.XAxis.Max.Value)
	//{
		m_Scope.XAxis.Max.Value = dlg.m_Max;
		m_Scope.XAxis.Max.DataValue = dlg.m_Max;
	//}
	//if(dlg.m_Min < m_Scope.XAxis.Min.Value)
	//{
		m_Scope.XAxis.Min.Value = dlg.m_Min;
		m_Scope.XAxis.Min.DataValue = dlg.m_Min;
	//}

	m_Scope.XAxis.ScaleMode = dlg.m_bLogEnable ? smLogarithmic : smLinear;
	m_Scope.XAxis.ZoomTo(dlg.m_Min,dlg.m_Max);
	m_Scope.Hold = FALSE;
}

void CDataBrowserDlg::OnAxisconfigureVerticalaxis()
{
	CAxisSettingsDlg dlg;
	
	dlg.m_Max = m_Scope.YAxis.CurrentMax;
	dlg.m_Min = m_Scope.YAxis.CurrentMin;
	dlg.m_Title = _T("Axis-Y(Height/m)");
	dlg.m_bLogCheckEnable = FALSE;
	if(IDOK != dlg.DoModal())
		return;

	m_Scope.YAxis.ZoomTo(dlg.m_Min,dlg.m_Max);
	m_Scope.Hold = FALSE;
}

void CDataBrowserDlg::OnAxisconfigureAixsAllReset()
{
	m_Scope.ZoomOff();
	m_Scope.Hold = FALSE;
}

void CDataBrowserDlg::OnNMClickRecordlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int cur = m_RecordList.GetSelectionMark();
	CString itemText = m_RecordList.GetItemText(cur,0);
	if(itemText.IsEmpty())
		return;

	CString path;
	m_BrowserEdit.GetWindowText(path);
	path = path + _T("\\") + itemText + _T(".rads");

	CDatRecord *pRecord = new CDatRecord();
	//pRecord->LoadItemData(path);
	PlotDat(*pRecord);

	pRecord->Clear();
	delete pRecord;
	
	*pResult = 0;
}

void CDataBrowserDlg::SetTimeRange(CTime& tStart,CTime& tEnd)
{
	m_DateStartPicker.SetTime(&tStart);
	m_TimeStartPicker.SetTime(&tStart);

	m_DateEndPicker.SetTime(&tEnd);
	m_TimeEndPicker.SetTime(&tEnd);
}

void CDataBrowserDlg::SetTimeRange(CTime& tStart,CTimeSpan tSpan)
{
	CTime tEnd = tStart + tSpan;
	SetTimeRange(tStart,tEnd);
}

void CDataBrowserDlg::OnBnClickedOk()
{
	int sum = m_RecordList.GetItemCount();
	if(sum == 0)
		return;

	m_pDatRecords = new vector<CDatRecord *>;
	CDatRecord *pRecord;
	for(int idx=0;idx<sum;idx++)
	{
		pRecord = (CDatRecord *)m_RecordList.GetItemData(idx);
		m_pDatRecords->push_back(pRecord);
	}

	CDialogEx::OnOK();
}

void CDataBrowserDlg::UpdateFilters()
{
	m_StationList.insert(_T("*"));
	m_ConductorList.insert(_T("*"));

	set<CString>::iterator it = m_StationList.begin();
	while(it != m_StationList.end())
	{
		m_ComboStation.AddString(*it);
		it++;
	}

	it = m_ConductorList.begin();
	while(it != m_ConductorList.end())
	{
		m_ComboConductor.AddString(*it);
		it++;
	}

	int nIndex = m_ComboStation.FindStringExact(0,_T("*"));
	m_ComboStation.SetCurSel(nIndex);

	nIndex = m_ComboConductor.FindStringExact(0,_T("*"));
	m_ComboConductor.SetCurSel(nIndex);
}  

void CDataBrowserDlg::OnBnClickedBtnSearchresult(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs)
{
	if(!pSender->IsKindOf(MARKUP_TYPE(CXTPMarkupHyperlink)))
		return;

	CString iniFile = Cradar_wspApp::GetAppPath() + _T("configs\\browser.ini");

	CTime tDateStart,tDateEnd,tTimeStart,tTimeEnd;
	m_DateStartPicker.GetTime(tDateStart);
	m_DateEndPicker.GetTime(tDateEnd);
	m_TimeStartPicker.GetTime(tTimeStart);
	m_TimeEndPicker.GetTime(tTimeEnd);
	
	CTime tStart(tDateStart.GetYear(),tDateStart.GetMonth(),tDateStart.GetDay(),tTimeStart.GetHour(),tTimeStart.GetMinute(),0);
	CTime tEnd(tDateEnd.GetYear(),tDateEnd.GetMonth(),tDateEnd.GetDay(),tTimeEnd.GetHour(),tTimeEnd.GetMinute(),0);
	if(tEnd < tStart)
	{
		MessageBox(_T("开始时间应早于结束时间!"),_T("错误"),MB_ICONERROR);
		return;
	}

	CString tempString;
	tempString = tStart.Format(_T("%Y/%m/%d-%H%M%S"));
	WritePrivateProfileString(_T("filter"),_T("tStart"),tempString,iniFile);
	tempString = tEnd.Format(_T("%Y/%m/%d-%H%M%S"));
	WritePrivateProfileString(_T("filter"),_T("tEnd"),tempString,iniFile);

	CString path;
	m_BrowserEdit.GetWindowText(path);
	WritePrivateProfileString(_T("localfiles"),_T("dir"),path,iniFile);

	unsigned int period = CAcquireHelper::AcquirePeriod_Value[m_ComboPeriod.GetCurSel()];
	QueryResult(tStart,tEnd,period);
}

void CDataBrowserDlg::OnBnClickedBtnHour(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs)
{
	CTime tStart = CTime::GetCurrentTime();
	CTimeSpan tSpan = 60*60;
	SetTimeRange(tStart-tSpan,tSpan);
}

void CDataBrowserDlg::OnBnClickedBtnSixhour(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs)
{
	CTime tStart = CTime::GetCurrentTime();
	CTimeSpan tSpan = 6*60*60;
	SetTimeRange(tStart-tSpan,tSpan);
}

void CDataBrowserDlg::OnBnClickedBtnHalfday(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs)
{
	CTime tStart = CTime::GetCurrentTime();
	CTimeSpan tSpan = 12*60*60;
	SetTimeRange(tStart-tSpan,tSpan);
}

void CDataBrowserDlg::OnBnClickedBtnDay(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs)
{
	CTime tStart = CTime::GetCurrentTime();
	CTimeSpan tSpan = 24*60*60;
	SetTimeRange(tStart-tSpan,tSpan);
}
