#pragma once
#include "ProductBaseView.h"
// CAdvancedProductView 窗体视图

class CAdvancedProductView : public CProductBaseView
{
	DECLARE_DYNCREATE(CAdvancedProductView)

public:
	CAdvancedProductView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAdvancedProductView();

public:
	void ScrollProductView();
	void ScrollProductView(CTime &tStart,CTime &tEnd);

	void UpdateView(CDatModel* pModel);
	void UpdateView(CDatModel* pModel,CProductHelper::ProductType product,CProductHelper::ProductChannelType ch = CProductHelper::ProductChannelType_1ch);
	void UpdateView(CDatRecord* pRecord);

	BOOL AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch = CProductHelper::ProductChannelType_1ch);
	void StartRealTimeView(CAcquireHelper* pAcquireHelper);

protected:
	void ReLayoutProductView();
	void ClearViewData();

	void __stdcall OnCustomDtAxesLabel(VCLHANDLE Sender, double Value, CTString &label);
	void __stdcall OnCustomHeightAxesLabel(VCLHANDLE Sender, double Value, CTString &label);
	void __stdcall OnTSLMouseMove(VCLHANDLE Sender,CTShiftState shift,int x,int y);
	void __stdcall OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y);
	void __stdcall OnTSLDataToolTip(VCLHANDLE Sender,POINT &pt,CTSLRealPoint &realPt,CTString &tooltip);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnAxisconfigureVerticalaxis();
	afx_msg void OnAxisconfigureHorizontalaxis();
	afx_msg void OnAxisconfigureLevelaxis();
	afx_msg void OnAxisconfigureAixsAllReset();
	afx_msg void OnAxisconfigureExportImg();
	afx_msg void OnAxisconfigurePrint();
};


