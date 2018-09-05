// MainFrm.h : interface of the CMainFrame class
//
#pragma once

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

protected:  
	CXTPSplitterWndEx m_wndSplitter;
	CXTPTaskPanel m_wndTaskPanel;
	CStatusBar  m_wndStatusBar;
	BOOL m_bInitial;
	CView* m_pAdvancedProductView;

protected:
	BOOL InitStatusBar();
	void SetCommandBarIcon();

protected:
	afx_msg LRESULT OnCaptureEvent(WPARAM wParam,LPARAM lParam);
	//afx_msg LRESULT OnCaptureTaskEvent(WPARAM wParam,LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCustomize();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	void UpdateWorkFlowState(int nIDString,int pos);
	void UpdateCursorPos(CString& posString);
	afx_msg void OnProductSignal1st();
	//afx_msg void OnProductSignal2nd();
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnProductExtinctioncoefficient1st();
	//afx_msg void OnProductAddcustomproductdisplaystyle();
	//afx_msg void OnProductExtinctioncoefficient2nd();
	afx_msg void OnProductDepolarRatio();
	//afx_msg void OnProductCloud();
	afx_msg void OnDataLocalfiles();
	afx_msg void OnCaptureSingle();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnOptionAdvance();
	afx_msg void OnOptionSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnProductSignal3rd();
	afx_msg void OnProductExtinctioncoefficient3rd();
	afx_msg void OnProductOpticdepth();
	afx_msg void OnProductCloud1st();
	afx_msg void OnProductAtmosboundary();
	afx_msg void OnProductVisible();
	//afx_msg void OnProductCloud2nd();
	afx_msg void OnProductCloud3rd();
	afx_msg void OnToolTaskeditor();
	afx_msg void OnCaptureScheduler();
	afx_msg void OnProductMixheight();
	afx_msg void OnProductMassconcentration();
	//afx_msg void OnProductSignal4th();
	//afx_msg void OnProductExtinctioncoefficient4th();
	//afx_msg void OnProductCloud4th();
	afx_msg void OnProductSignal5th();
	//afx_msg void OnProductSignal6th();
	afx_msg void OnProductExtinctioncoefficient5th();
	//afx_msg void OnProductExtinctioncoefficient6th();
	afx_msg void OnProductCloud5th();
	//afx_msg void OnProductCloud6th();
};
