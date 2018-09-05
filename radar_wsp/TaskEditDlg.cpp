// TaskEditDlg.cpp
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "TaskEditDlg.h"
#include "afxdialogex.h"
#include <algorithm>

char* CTaskEditDlg::WEEK_CAPTION[] = {"周一","周二","周三","周四","周五","周六","周日"};

IMPLEMENT_DYNAMIC(CTaskEditDlg, CDialogEx)

CTaskEditDlg::CTaskEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskEditDlg::IDD, pParent)
{
	
}

CTaskEditDlg::~CTaskEditDlg()
{

}

void CTaskEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GANTTCHART, m_wndChartControl);
	DDX_Control(pDX, IDC_SUBTASKLIST, m_SubTaskList);
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_TimeCtrlStart);
	DDX_Control(pDX, IDC_TIMEPICKER_END, m_TimeCtrlEnd);
	DDX_Control(pDX, IDC_COMBO_MODE, m_ComboMode);
	DDX_Control(pDX, IDC_COMBO_REPEAT, m_ComboRepeat);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_ComboCondition);
}

BEGIN_MESSAGE_MAP(CTaskEditDlg, CDialogEx)
	ON_COMMAND(ID_SCHEDULE_OPEN, &CTaskEditDlg::OnScheduleOpen)
	ON_LBN_SELCHANGE(IDC_SUBTASKLIST, &CTaskEditDlg::OnLbnSelchangeSubtasklist)
	ON_COMMAND(ID_SCHEDULE_NEW, &CTaskEditDlg::OnScheduleNew)
	ON_COMMAND(ID_SUBTASK_NEW, &CTaskEditDlg::OnSubtaskNew)
	ON_COMMAND(ID_SUBTASK_SAVE, &CTaskEditDlg::OnSubtaskSave)
	ON_COMMAND(ID_SCHEDULE_SAVE, &CTaskEditDlg::OnScheduleSave)
	ON_CBN_SELCHANGE(IDC_COMBO_REPEAT, &CTaskEditDlg::OnCbnSelchangeComboRepeat)
	ON_COMMAND(ID_SUBTASK_DELETE, &CTaskEditDlg::OnSubtaskDelete)
	ON_COMMAND(ID_SCHEDULE_SAVEAS, &CTaskEditDlg::OnScheduleSaveas)
END_MESSAGE_MAP()


void CTaskEditDlg::CreateChart()
{
	CXTPChartContent* pContent = m_wndChartControl.GetContent();
	pContent->GetLegend()->SetVisible(FALSE);

	CXTPChartSeries* pSeries = pContent->GetSeries()->Add(new CXTPChartSeries());
	pSeries->SetName(_T("子任务"));
	CXTPChartGanttSeriesStyle* pStyle = new CXTPChartGanttSeriesStyle();
	pStyle->SetBarWidth(0.3);
	pStyle->GetLabel()->SetVisible(FALSE);
	pSeries->SetStyle(pStyle);

	CXTPChartDiagram2D* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram2D, pSeries->GetDiagram());
	pDiagram->GetAxisX()->GetTitle()->SetVisible(FALSE);
	pDiagram->GetAxisY()->GetTitle()->SetVisible(FALSE);
	pDiagram->GetAxisY()->GetRange()->SetShowZeroLevel(FALSE);
	pDiagram->GetAxisY()->GetRange()->SetSideMargins(FALSE);
	pDiagram->GetAxisY()->GetRange()->SetAutoRange(FALSE);
	pDiagram->GetAxisY()->GetRange()->SetMaxValue(24);
	pDiagram->GetAxisY()->GetRange()->SetMinValue(0);

	pDiagram->SetRotated(TRUE);
}

BOOL CTaskEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CreateChart();
	SetSubTaskEditState(FALSE);

	return TRUE;
}

double CTaskEditDlg::GetTime(unsigned short hour,unsigned short minute)
{
	double t;
	t = minute;
	t /= 60.0;
	t += hour;
	return t;
}

void CTaskEditDlg::GetTime(double t,unsigned short &hour,unsigned short &minute)
{
	hour = t;
	int temp = t * 10;
	minute = (temp - hour*10) * 6;
}

BOOL CTaskEditDlg::CheckTime(PTR_SubTask pSubTask0,PTR_SubTask pSubTask1)
{
	if(pSubTask0->tEnd >= pSubTask1->tStart)
		return FALSE;
	else if(pSubTask1->tEnd >= pSubTask0->tStart)
		return FALSE;
	else return TRUE;
}

void CTaskEditDlg::CleatSubTasks()
{
	vector<PTR_SubTask>::iterator it = m_SubTasks.begin();
	while(it != m_SubTasks.end())
	{
		delete (*it);
		it++;
	}
	m_SubTasks.clear();
}

void CTaskEditDlg::ParserSubTask(int subTaskNum,PTR_SubTask pSubTask)
{
	CString sectCaption;
	sectCaption.Format(_T("subtask%d"),subTaskNum);

	CString strTemp;
	unsigned char cBuf[_MAX_PATH];

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("mode"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	//pSubTask->mode = (CAcquireHelper::AcquireMode_Type)(int)_ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("repeat"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->repeatType = (SubTaskRepeatType)(int)_ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("condition"),_T("1"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->repeatCondition = (short)_ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("init_fy"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->fy = _ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("init_fw"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->fw = _ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("step"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->step = _ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("period"),_T("60"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->period = _ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("tstart"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->tStart = _ttof((LPTSTR)(LPCTSTR)strTemp);

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(sectCaption,_T("tend"),_T("0.0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	pSubTask->tEnd = _ttof((LPTSTR)(LPCTSTR)strTemp);
}

void CTaskEditDlg::ParserSduFile()
{
	CString strTemp;
	unsigned char cBuf[_MAX_PATH];

	strTemp.Empty();
	memset(cBuf,0,_MAX_PATH);
	GetPrivateProfileString(_T("summary"),_T("sum"),_T("0"),(LPWSTR)cBuf,_MAX_PATH,m_ScheduleFilePath);
	strTemp.Append((LPWSTR)cBuf);
	int taskSum = _ttof((LPTSTR)(LPCTSTR)strTemp);

	CString sectionCaption;
	for(int i=0;i<taskSum;i++)
	{
		PTR_SubTask pSubTask = new SubTask;
		ParserSubTask(i,pSubTask);
		m_SubTasks.push_back(pSubTask);
	}
}

void CTaskEditDlg::SeriesSubTask(int subTaskNum,PTR_SubTask pSubTask)
{
	CString sectCaption;
	sectCaption.Format(_T("subtask%d"),subTaskNum);

	CString strTemp;
	//strTemp.Format(_T("%d"),pSubTask->mode);
	WritePrivateProfileString(sectCaption,_T("mode"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%d"),pSubTask->repeatType);
	WritePrivateProfileString(sectCaption,_T("repeat"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%d"),pSubTask->repeatCondition);
	WritePrivateProfileString(sectCaption,_T("condition"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%.1f"),pSubTask->fy);
	WritePrivateProfileString(sectCaption,_T("init_fy"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%.1f"),pSubTask->fw);
	WritePrivateProfileString(sectCaption,_T("init_fw"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%.1f"),pSubTask->step);
	WritePrivateProfileString(sectCaption,_T("step"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%d"),pSubTask->period);
	WritePrivateProfileString(sectCaption,_T("period"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%.2f"),pSubTask->tStart);
	WritePrivateProfileString(sectCaption,_T("tstart"),strTemp,m_ScheduleFilePath);

	strTemp.Empty();
	strTemp.Format(_T("%.2f"),pSubTask->tEnd);
	WritePrivateProfileString(sectCaption,_T("tend"),strTemp,m_ScheduleFilePath);
}

void CTaskEditDlg::SeriesToSduFile()
{
	int sum = m_SubTasks.size();
	CString strTemp;
	strTemp.Format(_T("%d"),sum);
	WritePrivateProfileString(_T("summary"),_T("sum"),strTemp,m_ScheduleFilePath);

	vector<PTR_SubTask>::iterator it = m_SubTasks.begin();
	int count = 0;
	while(it != m_SubTasks.end())
	{
		SeriesSubTask(count,(*it));
		count++;
		it++;
	}
}

void CTaskEditDlg::UpdateChart()
{
	CXTPChartContent* pContent = m_wndChartControl.GetContent();
	CXTPChartSeries* pSeries = pContent->GetSeries()->GetAt(0);
	pSeries->GetPoints()->RemoveAll();

	CString itemString;
	unsigned short starthour,startminute,endhour,endminute;
	vector<PTR_SubTask>::iterator it = m_SubTasks.begin();
	while(it != m_SubTasks.end())
	{
		GetTime((*it)->tStart,starthour,startminute);
		GetTime((*it)->tEnd,endhour,endminute);
		itemString.Format(_T("%02d:%02d-%02d:%02d "),starthour,startminute,endhour,endminute);
		/*if((*it)->mode == CAcquireHelper::AcquireMode_Fixed)
			itemString += _T("定向采集");
		else itemString += _T("PPI采集");*/
		CXTPChartContent* pContent = m_wndChartControl.GetContent();
		CXTPChartSeries* pSeries = pContent->GetSeries()->GetAt(0);
		pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(itemString,(*it)->tStart,(*it)->tEnd));
		
		itemString.Empty();
		it++;
	}
}

void CTaskEditDlg::OnLbnSelchangeSubtasklist()
{
	if(!m_SubTaskList.GetCount())
		return;
	int cur = m_SubTaskList.GetCurSel();
	if(cur >= m_SubTasks.size())
		return;
	PTR_SubTask pSubTask = m_SubTasks.at(cur);
	UpdateSubTaskEdit(pSubTask);
}

void CTaskEditDlg::OnCbnSelchangeComboRepeat()
{
	m_ComboCondition.ResetContent();
	SubTaskRepeatType repeatType = (SubTaskRepeatType)(m_ComboRepeat.GetCurSel());
	if(repeatType == SubTaskRepeat_Week)
	{
		for(int i=0;i<7;i++)
		{
			CString strItem(WEEK_CAPTION[i]);
			m_ComboCondition.AddString(strItem);
		}
		m_ComboCondition.SetCurSel(0);
		m_ComboCondition.EnableWindow(TRUE);
	}
	else if(repeatType == SubTaskRepeat_Month)
	{
		CString strItem;
		for(int i=0;i<31;i++)
		{
			strItem.Format(_T("%d日"),i+1);
			m_ComboCondition.AddString(strItem);
		}
		m_ComboCondition.SetCurSel(0);
		m_ComboCondition.EnableWindow(TRUE);
	}
	else m_ComboCondition.EnableWindow(FALSE);
}

void CTaskEditDlg::UpdateSubTaskEdit(PTR_SubTask pSubTask)
{
	//m_ComboMode.SetCurSel(pSubTask->mode);
	m_ComboRepeat.SetCurSel(pSubTask->repeatType);

	m_ComboCondition.ResetContent();
	if(pSubTask->repeatType == SubTaskRepeat_Week)
	{
		for(int i=0;i<7;i++)
		{
			CString strItem(WEEK_CAPTION[i]);
			m_ComboCondition.AddString(strItem);
		}
		m_ComboCondition.SetCurSel(pSubTask->repeatCondition-1);
		m_ComboCondition.EnableWindow(TRUE);
	}
	else if(pSubTask->repeatType == SubTaskRepeat_Month)
	{
		CString strItem;
		for(int i=0;i<31;i++)
		{
			strItem.Format(_T("%d日"),i+1);
			m_ComboCondition.AddString(strItem);
		}
		m_ComboCondition.SetCurSel(pSubTask->repeatCondition-1);
		m_ComboCondition.EnableWindow(TRUE);
	}
	else m_ComboCondition.EnableWindow(FALSE);

	unsigned short hour,minute;
	GetTime(pSubTask->tStart,hour,minute);
	CTime tStart(1990,1,1,hour,minute,0);
	m_TimeCtrlStart.SetTime(&tStart);
	GetTime(pSubTask->tEnd,hour,minute);
	CTime tEnd(1990,1,1,hour,minute,0);
	m_TimeCtrlEnd.SetTime(&tEnd);

	CString strWindowText;
	strWindowText.Format(_T("%.2f"),pSubTask->fy);
	GetDlgItem(IDC_EDIT_INITFY)->SetWindowText(strWindowText);

	strWindowText.Format(_T("%.2f"),pSubTask->fw);
	GetDlgItem(IDC_EDIT_INITFW)->SetWindowText(strWindowText);

	strWindowText.Format(_T("%.2f"),pSubTask->step);
	GetDlgItem(IDC_EDIT_STEP)->SetWindowText(strWindowText);

	strWindowText.Format(_T("%d"),pSubTask->period);
	GetDlgItem(IDC_EDIT_PERIOD)->SetWindowText(strWindowText);
}

void CTaskEditDlg::GetSubTaskEdit(PTR_SubTask pSubTask)
{
	//pSubTask->mode = (CAcquireHelper::AcquireMode_Type)m_ComboMode.GetCurSel();
	pSubTask->repeatType = (SubTaskRepeatType)m_ComboRepeat.GetCurSel();
	pSubTask->repeatCondition = m_ComboCondition.GetCurSel()+1;
	CString szWindowText;
	GetDlgItem(IDC_EDIT_INITFY)->GetWindowText(szWindowText);
	pSubTask->fy = _ttof((LPTSTR)(LPCTSTR)szWindowText);
	GetDlgItem(IDC_EDIT_INITFW)->GetWindowText(szWindowText);
	pSubTask->fw = _ttof((LPTSTR)(LPCTSTR)szWindowText);
	GetDlgItem(IDC_EDIT_STEP)->GetWindowText(szWindowText);
	pSubTask->step = _ttof((LPTSTR)(LPCTSTR)szWindowText);
	GetDlgItem(IDC_EDIT_PERIOD)->GetWindowText(szWindowText);
	pSubTask->period = _ttof((LPTSTR)(LPCTSTR)szWindowText);

	CTime tStart,tEnd;
	m_TimeCtrlStart.GetTime(tStart);
	pSubTask->tStart = GetTime(tStart.GetHour(),tStart.GetMinute());
	m_TimeCtrlEnd.GetTime(tEnd);
	pSubTask->tEnd = GetTime(tEnd.GetHour(),tEnd.GetMinute());
}

void CTaskEditDlg::SetSubTaskEditState(BOOL bEnable)
{
	if(bEnable)
	{
		m_ComboMode.ResetContent();
		m_ComboMode.EnableWindow(TRUE);
		m_ComboMode.AddString(_T("定向采集"));
		m_ComboMode.AddString(_T("PPI采集"));

		m_ComboRepeat.ResetContent();
		m_ComboRepeat.EnableWindow(TRUE);
		m_ComboRepeat.AddString(_T("无"));
		m_ComboRepeat.AddString(_T("每天"));
		m_ComboRepeat.AddString(_T("每周"));
		m_ComboRepeat.AddString(_T("每月"));

		m_ComboCondition.EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_INITFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INITFW)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(TRUE);
	}
	else
	{
		m_ComboMode.ResetContent();
		m_ComboRepeat.ResetContent();
		m_ComboCondition.ResetContent();
		GetDlgItem(IDC_EDIT_INITFY)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_INITFW)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_STEP)->SetWindowText(_T(""));
		GetDlgItem(IDC_EDIT_PERIOD)->SetWindowText(_T(""));

		m_ComboMode.EnableWindow(FALSE);
		m_ComboRepeat.EnableWindow(FALSE);
		m_ComboCondition.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INITFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INITFW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
	}
}

void CTaskEditDlg::UpdateSubTaskListCtrl(int sel)
{
	m_SubTaskList.ResetContent();
	CString itemString;
	unsigned short starthour,startminute,endhour,endminute;
	vector<PTR_SubTask>::iterator it = m_SubTasks.begin();
	int idx=0;
	while(it != m_SubTasks.end())
	{
		GetTime((*it)->tStart,starthour,startminute);
		GetTime((*it)->tEnd,endhour,endminute);
		itemString.Format(_T("%02d:%02d-%02d:%02d "),starthour,startminute,endhour,endminute);
		/*if((*it)->mode == CAcquireHelper::AcquireMode_Fixed)
			itemString += _T("single");
		else itemString += _T("PPI");*/
		m_SubTaskList.AddString(itemString);
		itemString.Empty();
		it++;
		idx++;
	}

	if(!m_SubTaskList.GetCount())
	{
		SetSubTaskEditState(FALSE);
		return;
	}
	else SetSubTaskEditState(TRUE);

	if(sel >=0 && sel < m_SubTaskList.GetCount())
		m_SubTaskList.SetCurSel(sel);
	else if(sel == -1)
		m_SubTaskList.SetCurSel(m_SubTaskList.GetCount()-1);

	OnLbnSelchangeSubtasklist();
}

void CTaskEditDlg::OnScheduleOpen()
{
	CString defaultName = _T(".sdu");
    CString filter = _T("采集计划文件(*.sdu)|*.sdu||"); 
	CFileDialog fDialog(TRUE,NULL,defaultName,OFN_HIDEREADONLY|OFN_READONLY,filter);
	if(fDialog.DoModal() != IDOK)
		return;
	m_ScheduleFilePath = fDialog.GetPathName();

	SetWindowText(_T("采集计划编辑器(")+m_ScheduleFilePath+_T(")"));

	CleatSubTasks();
	ParserSduFile();
	UpdateChart();
	UpdateSubTaskListCtrl(0);
}

void CTaskEditDlg::OnScheduleNew()
{
	CString defaultName = _T("新建采集计划文件.sdu");
    CString filter = _T("采集计划文件(*.sdu)|*.sdu||"); 
	CFileDialog fDialog(FALSE,NULL,defaultName,OFN_HIDEREADONLY|OFN_READONLY,filter);
	if(fDialog.DoModal() != IDOK)
		return;
	m_ScheduleFilePath = fDialog.GetPathName();

	CFile f;
	f.Open(m_ScheduleFilePath,CFile::modeCreate);
	f.Close();

	SetWindowText(_T("采集计划编辑器(")+m_ScheduleFilePath+_T(")"));

	CleatSubTasks();
	UpdateChart();
	UpdateSubTaskListCtrl(0);
}

void CTaskEditDlg::OnScheduleSave()
{
	std::sort(m_SubTasks.begin(),m_SubTasks.end(),SubTask::comp); 
	UpdateChart();
	UpdateSubTaskListCtrl(-1);
	SeriesToSduFile();
}

void CTaskEditDlg::OnScheduleSaveas()
{
	CString defaultName = _T("新建采集计划文件.sdu");
    CString filter = _T("采集计划文件(*.sdu)|*.sdu||"); 
	CFileDialog fDialog(FALSE,NULL,defaultName,OFN_HIDEREADONLY|OFN_READONLY,filter);
	if(fDialog.DoModal() != IDOK)
		return;
	m_ScheduleFilePath = fDialog.GetPathName();

	CFile f;
	f.Open(m_ScheduleFilePath,CFile::modeCreate);
	f.Close();

	SetWindowText(_T("采集计划编辑器(")+m_ScheduleFilePath+_T(")"));

	SeriesToSduFile();
}

void CTaskEditDlg::OnSubtaskNew()
{
	PTR_SubTask pSubTask = new SubTask;
	m_SubTasks.push_back(pSubTask);
	UpdateChart();
	UpdateSubTaskListCtrl(-1);
}

void CTaskEditDlg::OnSubtaskSave()
{
	int cur = m_SubTaskList.GetCurSel();
	PTR_SubTask pSubTask = m_SubTasks.at(cur);
	GetSubTaskEdit(pSubTask);
	UpdateChart();
	UpdateSubTaskListCtrl(cur);
}

void CTaskEditDlg::OnSubtaskDelete()
{
	int cur = m_SubTaskList.GetCurSel();
	m_SubTaskList.DeleteString(cur);

	int idx = 0;
	vector<PTR_SubTask>::iterator it = m_SubTasks.begin();
	while(it != m_SubTasks.end())
	{
		if(idx == cur)
		{
			m_SubTasks.erase(it);
			break;
		}
		idx++;
		it++;
	}
	UpdateChart();
	UpdateSubTaskListCtrl(cur);
}


