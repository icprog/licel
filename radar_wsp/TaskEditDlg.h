#pragma once
#include "afxwin.h"
#include <vector>
#include "CaptureTaskDlg.h"
#include "afxdtctl.h"
using namespace std;

// CTaskEditDlg �Ի���

class CTaskEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskEditDlg)

public:
	CTaskEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskEditDlg();

// �Ի�������
	enum { IDD = IDD_TASKEDITDLG };

	typedef enum tagSubTaskRepeatType
	{
		SubTaskRepeat_None = 0,
		SubTaskRepeat_Day,
		SubTaskRepeat_Week,
		SubTaskRepeat_Month
	}SubTaskRepeatType;

	typedef struct tagSubTask
	{
		//CAcquireHelper::AcquireMode_Type mode;
		SubTaskRepeatType repeatType;
		short repeatCondition;
		double fy;
		double fw;
		double step;
		unsigned short period;
		double tStart;
		double tEnd;	

		tagSubTask()
		{
			//mode = CAcquireHelper::AcquireMode_Fixed;
			repeatType = SubTaskRepeat_None;
			repeatCondition = 0;
			fy=0.0;
			fw=0.0;
			step=0.5;
			period=60;
			tStart=0.0;
			tEnd=0.0;
		}

		static BOOL comp(tagSubTask* p1,tagSubTask* p2)
		{
			if(p1->tStart > p2->tStart)
				return FALSE;
			else return TRUE;
		}
	}SubTask,*PTR_SubTask;
	static char* WEEK_CAPTION[7];

protected:
	vector<PTR_SubTask> m_SubTasks;
	CString m_ScheduleFilePath;

	CXTPChartControl m_wndChartControl;
	CListBox m_SubTaskList;
	CDateTimeCtrl m_TimeCtrlStart;
	CDateTimeCtrl m_TimeCtrlEnd;
	CComboBox m_ComboMode;
	CComboBox m_ComboRepeat;
	CComboBox m_ComboCondition;

protected:
	static double GetTime(unsigned short hour,unsigned short minute);
	static void GetTime(double t,unsigned short &hour,unsigned short &minute);
	static BOOL CheckTime(PTR_SubTask pSubTask0,PTR_SubTask pSubTask1);
	void ParserSubTask(int subTaskNum,PTR_SubTask pSubTask);
	void ParserSduFile();
	void SeriesSubTask(int subTaskNum,PTR_SubTask pSubTask);
	void SeriesToSduFile();
	void CreateChart();
	void UpdateChart();
	void CleatSubTasks();
	void UpdateSubTaskEdit(PTR_SubTask pSubTask);
	void GetSubTaskEdit(PTR_SubTask pSubTask);
	void SetSubTaskEditState(BOOL bEnable);
	void UpdateSubTaskListCtrl(int sel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnScheduleOpen();
	afx_msg void OnLbnSelchangeSubtasklist();
	afx_msg void OnScheduleNew();
	afx_msg void OnSubtaskNew();
	afx_msg void OnSubtaskSave();
	afx_msg void OnScheduleSave();
	afx_msg void OnCbnSelchangeComboRepeat();
	afx_msg void OnSubtaskDelete();
	afx_msg void OnScheduleSaveas();
};
