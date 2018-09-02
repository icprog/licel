// radar_wspView.h : interface of the Cradar_wspView class
//


#pragma once


class Cradar_wspView : public CView
{
protected: // create from serialization only
	Cradar_wspView();
	DECLARE_DYNCREATE(Cradar_wspView)

// Attributes
public:
	CRadar_wspDoc* GetDocument() const;

protected:
	BOOL m_bInitial;
	CTSLWaterfall m_Waterfall;
	CTSLScope m_Scope;

// Operations
protected:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cradar_wspView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in radar_wspView.cpp
inline Cradar_wspDoc* Cradar_wspView::GetDocument() const
   { return reinterpret_cast<Cradar_wspDoc*>(m_pDocument); }
#endif
