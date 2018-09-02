#pragma once
#include "ProductBaseView.h"

class CRadarFormView : public CProductBaseView
{
	DECLARE_DYNCREATE(CRadarFormView)

protected:
	CRadarFormView();           
	virtual ~CRadarFormView();

public:
	void ScrollProductView();
	void ScrollProductView(CTime &tStart,CTime &tEnd);

	BOOL AddProductView(CProductHelper::ProductType product,unsigned int ch=0);

	void UpdateView(CDatModel* pModel);
	void UpdateView(CDatModel* pModel,CProductHelper::ProductType product,unsigned int ch=0);
	void UpdateView(CDatRecord* pRecord,BOOL bWaterFall = FALSE);
	void StartRealTimeView(CAcquireHelper* pAcquireHelper);

protected:
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
};


