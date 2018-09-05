#pragma once
#include "ProductBaseView.h"
// CAdvancedProductView 窗体视图

class CAdvancedProductView : public CProductBaseView
{
	DECLARE_DYNCREATE(CAdvancedProductView)

public:
	CAdvancedProductView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAdvancedProductView();

	static unsigned int PRODUCT_CLOUD_COLOR[4]; 
	
protected:
	VCLHANDLE m_ContetxMenuSender;

public:
	void ScrollProductView();
	void ScrollProductView(CTime &tStart,CTime &tEnd);

	void UpdateView(CDatModel* pModel);
	BOOL AddProduct(CProductHelper::ProductType product,unsigned int ch);

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
	afx_msg void OnAxisconfigureCh1();
	afx_msg void OnAxisconfigureCh2();
	afx_msg void OnAxisconfigureCh3();
	afx_msg void OnAxisconfigureVerticalaxis();
};


