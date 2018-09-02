// radar_wspView.cpp : implementation of the Cradar_wspView class
//

#include "stdafx.h"
#include "radar_wsp.h"

#include "radar_wspDoc.h"
#include "radar_wspView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cradar_wspView

IMPLEMENT_DYNCREATE(Cradar_wspView, CView)

BEGIN_MESSAGE_MAP(Cradar_wspView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// Cradar_wspView construction/destruction

Cradar_wspView::Cradar_wspView()
{
	m_bInitial = FALSE;

}

Cradar_wspView::~Cradar_wspView()
{
}

BOOL Cradar_wspView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cradar_wspView drawing

void Cradar_wspView::OnDraw(CDC* /*pDC*/)
{
	CRadar_wspDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	// TODO: add draw code for native data here
}


// Cradar_wspView printing

BOOL Cradar_wspView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cradar_wspView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cradar_wspView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cradar_wspView diagnostics

#ifdef _DEBUG
void Cradar_wspView::AssertValid() const
{
	CView::AssertValid();
}

void Cradar_wspView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRadar_wspDoc* Cradar_wspView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRadar_wspDoc)));
	return (CRadar_wspDoc*)m_pDocument;
}
#endif //_DEBUG


// Cradar_wspView message handlers


void Cradar_wspView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_Waterfall.Open(this->m_hWnd);
	//m_Waterfall.Align= CTAlign::alNone;
	//m_Waterfall.Align = CTAlign::alCustom;
	m_Waterfall.ToolBar.Visible = false;
	m_Waterfall.Title.Text = _T("orgin signal");
	m_Waterfall.Hold = false;
	m_Waterfall.Data.Clear();
	m_Waterfall.Levels.Axis.ScaleMode = CTSLScaleMode::smLogarithmic;
	m_Waterfall.XAxis.AxisLabel.Text = _T("sample time");
	m_Waterfall.YAxis.AxisLabel.Text = _T("altitude(km)");
	
	m_Scope.Open(this->m_hWnd);
	//m_Scope.Align = CTAlign::alCustom;
	m_Scope.Title.Text = _T("orgin signal");
	m_Scope.Channels.Add();
	m_Scope.Channels[ 0 ].Name = _T("Random");
	
	VCL_Loaded();

	m_bInitial = TRUE;
}


void Cradar_wspView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_bInitial)
	{
		
		m_Waterfall.Left = 0;
		m_Waterfall.Top = 0;
		m_Waterfall.Height = cy;
		m_Waterfall.Width = cx*3/4;
		m_Waterfall.Invalidate();
		
		
		m_Scope.Left = cx*3/4;
		m_Scope.Top = 0;
		m_Scope.Height = cy;
		m_Scope.Width = cx/4;
		
	}
}
