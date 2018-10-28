#pragma once

class CProductBaseView : public CFormView
{
	DECLARE_DYNCREATE(CProductBaseView)

protected:
	CProductBaseView();           
	virtual ~CProductBaseView();

public:
	enum { IDD = IDD_PRODUCTBASEVIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	typedef struct tagProductView
	{
		CProductHelper::ProductType m_ProductType;
		CProductHelper::ProductChannelType m_Ch;
		CStatic *m_pStatic[2];
		CTSLWaterfall *m_pWaterfall;
		CTSLScope *m_pScope;

		tagProductView()
		{
			m_Ch = CProductHelper::ProductChannelType_1ch;
			m_pStatic[0] = NULL;
			m_pStatic[1] = NULL;
			m_pWaterfall = NULL;
			m_pScope = NULL;
		}
	}ProductView,*PTR_ProductView;
	
	enum {MAX_PRODUCT_VIEW_COUNT = 6};

protected:
	// 产品曲线开始时间
	CTime m_tStart;
	CTimeSpan m_tSpan;
	unsigned int m_CurvesPerScreen;
	unsigned int m_heightInterval;
	//时间分辨率
	unsigned int m_datetimeInterval;
	//采样点数
	unsigned int m_curvePointsSize;

	PTR_ProductView m_pProductView[MAX_PRODUCT_VIEW_COUNT];
	CTSLWaterfall *m_pSelectedWaterfall;
	CTSLScope *m_pSelectedScope;
	CProductHelper::ProductType m_SelectProductType;
	CProductHelper::ProductChannelType m_SelectChannelType;
	CDatRecord *m_pHookedRecord;

	BOOL m_bInitial;

public:
	virtual void StartRealTimeView(CAcquireHelper* pAcquireHelper);
	virtual BOOL AddProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);
	BOOL DelProductView(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);
	BOOL IsProductViewExist(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);
	
protected:
	void ClearViewData();
	int GetIdealProductElement();
	int GetValidProductElementCount();
	int GetProductViewIdx(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch);
	void SaveScreenImg();
	CString BuildScreenImgFolder(CTime &tInfo,CProductHelper::ProductType productType);
	virtual void ReLayoutProductView();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
};


