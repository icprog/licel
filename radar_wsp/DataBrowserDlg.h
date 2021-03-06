#pragma once
//#include "afxwin.h"
//#include "afxcmn.h"
#include "CaptureTaskDlg.h"
#include <vector>
#include <set>
#include "afxdtctl.h"
#include "afxwin.h"
using namespace std;


class CDataBrowserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataBrowserDlg)

public:
	CDataBrowserDlg(CWnd* pParent = NULL); 
	virtual ~CDataBrowserDlg();

	enum { IDD = IDD_DATABROWERDLG };

public:
	vector<CDatRecord *> *m_pDatRecords;

protected:
	CXTPBrowseEdit m_BrowserEdit;
	CComboBox m_ComboPeriod;
	CListCtrl m_RecordList;
	CTSLScope m_Scope;

	CDateTimeCtrl m_DateStartPicker;
	CDateTimeCtrl m_TimeStartPicker;
	CDateTimeCtrl m_DateEndPicker;
	CDateTimeCtrl m_TimeEndPicker;

	CXTPButton m_BtnHour;
	CXTPButton m_BtnSixHour;
	CXTPButton m_BtnHalfDay;
	CXTPButton m_BtnDay;

	CXTPButton m_BtnSearchResult;

	CStatusBar m_wndStatusBar;
	CProgressCtrl m_ProgressCtrl;

	set<CString> m_StationList;
	set<CString> m_ConductorList;

	CComboBox m_ComboStation;
	CComboBox m_ComboConductor;

	void PlotDat(CDatRecord &record);
	void __stdcall OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y);
	BOOL Record2List(CDatRecord *pRecord,CString station,CString conductor);
	void QueryFolder(CString folder,CTime& tStart,CTime& tEnd,unsigned int period);
	void QueryResult(CTime& tStart,CTime& tEnd,unsigned int period);
	void ClearRecordList();
	void UpdateFilters();
	
protected:
	void SetTimeRange(CTime& tStart,CTime& tEnd);
	void SetTimeRange(CTime& tStart,CTimeSpan tSpan);
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnAxisconfigureHorizontalaxis();
	afx_msg void OnAxisconfigureVerticalaxis();
	afx_msg void OnAxisconfigureAixsAllReset();
	afx_msg void OnNMClickRecordlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	void OnBnClickedBtnSearchresult(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs);
	void OnBnClickedBtnHour(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs);
	void OnBnClickedBtnSixhour(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs);
	void OnBnClickedBtnHalfday(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs);
	void OnBnClickedBtnDay(CXTPMarkupObject* pSender,CXTPMarkupRoutedEventArgs* pArgs);
};
