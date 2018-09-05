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
		unsigned int m_Ch;
		CStatic *m_pStatic[2];
		CTSLWaterfall *m_pWaterfall;
		CTSLScope *m_pScope;

		tagProductView()
		{
			m_Ch = 0;
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
	// 产品曲线持续时间
	unsigned int m_tSpan;

	//距离分辨率
	unsigned int m_heightInterval;
	//时间分辨率
	unsigned int m_datetimeInterval;
	//采样点数
	unsigned int m_curvePointsSize;
	//距离库
	double* m_pRange;

	PTR_ProductView m_pProductView[MAX_PRODUCT_VIEW_COUNT];
	CTSLWaterfall *m_pSelectedWaterfall;
	CTSLScope *m_pSelectedScope;
	BOOL m_bInitial;

public:
	virtual BOOL AddProductView(CProductHelper::ProductType product,unsigned int ch=0);
	BOOL DelProductView(CProductHelper::ProductType product,unsigned int ch=0);
	BOOL IsProductViewExist(CProductHelper::ProductType product,unsigned int ch=0);
	
protected:
	void ClearViewData();
	int GetIdealProductElement();
	int GetValidProductElementCount();
	int GetProductViewIdx(CProductHelper::ProductType product,unsigned int ch);
	virtual void ReLayoutProductView();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
};


