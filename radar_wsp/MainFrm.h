// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "CaptureTaskDlg.h"
#include "ColorStatusBar.h"
class CMainFrame : public CXTPFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
protected:
	 BOOL CreateTaskPanel();
	 void CreateTaskPanelItems();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CProgressCtrl m_ProgressCtrl;
	CCaptureTaskDlg *m_pCaptureTaskDlg;

protected:  
	CXTPSplitterWndEx m_wndSplitter;
	CXTPTaskPanel m_wndTaskPanel;
	CColorStatusBar m_wndStatusBar;
	BOOL m_bInitial;
	CView* m_pAdvancedProductView;

protected:
	BOOL InitStatusBar();
	void SetCommandBarIcon();
	
public:
	void UpdateCursorPos(CString& posString);
	void UpdateWorkFlowState(int nIDString,int pos);

protected:
	afx_msg LRESULT OnCaptureEvent(WPARAM wParam,LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	afx_msg void OnProductSignal1st();
	afx_msg void OnProductSignal2nd();
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProductExtinctioncoefficient1st();
	afx_msg void OnProductExtinctioncoefficient2nd();
	afx_msg void OnProductDepolarRatio();
	afx_msg void OnDataLocalfiles();
	afx_msg void OnCaptureSingle();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnOptionAdvance();
	afx_msg void OnOptionSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnProductSignal3rd();
	afx_msg void OnProductExtinctioncoefficient3rd();
	afx_msg void OnProductOpticdepth();
	afx_msg void OnProductCloud();
	afx_msg void OnProductAtmosboundary();
	afx_msg void OnToolTaskeditor();
	afx_msg void OnCaptureScheduler();
	afx_msg void OnProductMixheight();
	afx_msg void OnProductMassconcentration();
	afx_msg void OnProductViewswitch();
	afx_msg void OnProductVisiblity1st();
	afx_msg void OnProductVisiblity2nd();
	afx_msg void OnProductVisiblity3rd();
	afx_msg void OnProductBackscatter1st();
	afx_msg void OnProductBackscatter2nd();
	afx_msg void OnProductBackscatter3rd();
	afx_msg void OnToolScreencopy();
};
