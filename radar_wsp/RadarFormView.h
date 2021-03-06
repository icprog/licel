#pragma once
#include "ProductBaseView.h"

class CRadarFormView : public CProductBaseView
{
	DECLARE_DYNCREATE(CRadarFormView)

protected:
	CRadarFormView();           
	virtual ~CRadarFormView();

public:
	// 翻屏
	void ScrollProductView();
	// 翻屏
	void ScrollProductView(CTime &tStart,CTime &tEnd);

	// 添加产品曲线
	BOOL AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);

	// 使用模型刷新产品曲线
	void UpdateView(CDatModel* pModel);
	// 使用模型追加刷新产品曲线
	void UpdateView(CDatModel* pModel,CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);
	// 使用记录刷新产品曲线
	void UpdateView(CDatRecord* pRecord,BOOL bWaterFall = FALSE);
	// 开始实时刷新
	void StartRealTimeView(CAcquireHelper* pAcquireHelper);

protected:
	// 重新计算产品曲线布局
	void ReLayoutProductView();

	void __stdcall OnCustomDtAxesLabel(VCLHANDLE Sender, double Value, CTString &label);
	void __stdcall OnCustomHeightAxesLabel(VCLHANDLE Sender, double Value, CTString &label);
	void __stdcall OnTSLMouseMove(VCLHANDLE Sender,CTShiftState shift,int x,int y);
	void __stdcall OnTSLMouseUp(VCLHANDLE Sender,CTMouseButton btn,CTShiftState shift,int x,int y);
	void __stdcall OnTSLDataToolTip(VCLHANDLE Sender,POINT &pt,CTSLRealPoint &realPt,CTString &tooltip);

	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAxisconfigureHorizontalaxis();
	afx_msg void OnAxisconfigureVerticalaxis();
	afx_msg void OnAxisconfigureLevelaxis();
	afx_msg void OnAxisconfigureAixsAllReset();
	afx_msg void OnAxisconfigureExportImg();
	afx_msg void OnAxisconfigurePrint();
	afx_msg void OnAxisconfigureTable();
};


