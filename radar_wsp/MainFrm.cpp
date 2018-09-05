// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "MainFrm.h"
#include "Radar_wspDoc.h"
#include "RadarFormView.h"
#include "AdvancedProductView.h"
#include "DataBrowserDlg.h"
#include "SaveSettingsDlg.h"
#include "TaskEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_COMMAND(ID_PRODUCT_SIGNAL_1ST,OnProductSignal1st)
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
	ON_COMMAND(ID_DATA_LOCALFILES, &CMainFrame::OnDataLocalfiles)
	ON_COMMAND(ID_PRODUCT_EXTINCTIONCOEFFICIENT_1ST, &CMainFrame::OnProductExtinctioncoefficient1st)
	ON_MESSAGE(WM_CAPTUREEVENT,&CMainFrame::OnCaptureEvent)
	ON_COMMAND(ID_PRODUCT_DEPOLAR_RATIO, &CMainFrame::OnProductDepolarRatio)
	ON_WM_TIMER()
	ON_COMMAND(ID_OPTION_ADVANCE, &CMainFrame::OnOptionAdvance)
	ON_COMMAND(ID_OPTION_SAVE, &CMainFrame::OnOptionSave)
	ON_COMMAND(ID_CAPTURE_SINGLE, &CMainFrame::OnCaptureSingle)
	ON_WM_SIZE()
	ON_COMMAND(ID_PRODUCT_SIGNAL_3RD, &CMainFrame::OnProductSignal3rd)
	ON_COMMAND(ID_PRODUCT_EXTINCTIONCOEFFICIENT_3RD, &CMainFrame::OnProductExtinctioncoefficient3rd)
	ON_COMMAND(ID_PRODUCT_OPTICDEPTH, &CMainFrame::OnProductOpticdepth)
	ON_COMMAND(ID_PRODUCT_CLOUD_1ST, &CMainFrame::OnProductCloud1st)
	ON_COMMAND(ID_PRODUCT_ATMOSBOUNDARY, &CMainFrame::OnProductAtmosboundary)
	ON_COMMAND(ID_PRODUCT_VISIBLE, &CMainFrame::OnProductVisible)
	ON_COMMAND(ID_PRODUCT_CLOUD_3RD, &CMainFrame::OnProductCloud3rd)
	ON_COMMAND(ID_TOOL_TASKEDITOR, &CMainFrame::OnToolTaskeditor)
	ON_COMMAND(ID_CAPTURE_SCHEDULER, &CMainFrame::OnCaptureScheduler)
	ON_COMMAND(ID_PRODUCT_MIXHEIGHT, &CMainFrame::OnProductMixheight)
	ON_COMMAND(ID_PRODUCT_MASSCONCENTRATION, &CMainFrame::OnProductMassconcentration)
	ON_COMMAND(ID_PRODUCT_SIGNAL_5TH, &CMainFrame::OnProductSignal5th)
	ON_COMMAND(ID_PRODUCT_EXTINCTIONCOEFFICIENT_5TH, &CMainFrame::OnProductExtinctioncoefficient5th)
	ON_COMMAND(ID_PRODUCT_CLOUD_5TH, &CMainFrame::OnProductCloud5th)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	//ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MOUSEPOS,
	ID_INDICATOR_SOURCE,
	ID_INDICATOR_DOWNLOAD,
	ID_INDICATOR_ANGLE,
	ID_INDICATOR_LASER,
	ID_INDICATOR_MAINPROGRESS,
};

static UINT uHideCmds[] =
{
	ID_FILE_PRINT,
	ID_FILE_PRINT_PREVIEW,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bInitial = FALSE;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!InitStatusBar())
		return -1;
	// Initialize the command bars
	if (!InitCommandBars())
		return -1;

	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}

	// Add the menu bar
	CXTPCommandBar* pMenuBar = pCommandBars->SetMenu(
		_T("Menu Bar"), IDR_MAINFRAME);
	if(pMenuBar == NULL)
	{
		TRACE0("Failed to create menu bar.\n");
		return -1;      // fail to create
	}

	// Create ToolBar
	CXTPToolBar* pMainToolBar = (CXTPToolBar*)
		pCommandBars->Add(_T("��׼"), xtpBarTop);
	if (!pMainToolBar || !pMainToolBar->LoadToolBar(IDR_MAINFRAME,0))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}
	pMainToolBar->EnableDocking(xtpFlagAlignAny | xtpFlagFloating | /*xtpFlagStretched |*/ xtpFlagHideWrap);

	SetCommandBarIcon();

	// Set Windows XP Theme
	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);
	//CString styleFile = Cradar_wspApp::GetAppPath() + _T("Styles\\");
	//XTPSkinManager()->LoadSkin(styleFile + _T("Codejock.cjstyles"));
	//XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	//XTPSkinManager()->LoadSkin(styleFile + _T("Codejock.cjstyles"));	


	// Hide array of commands
	pCommandBars->HideCommands(uHideCmds, _countof(uHideCmds));

	// Set "Always Show Full Menus" option to the FALSE
	pCommandBars->GetCommandBarsOptions()->bAlwaysShowFullMenus = FALSE;

	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);

	// Load the previous state for toolbars and menus.
	LoadCommandBars(_T("CommandBars"));

	m_bInitial = TRUE;

	SetTimer(1,200,NULL);

	return 0;
}

BOOL CMainFrame::InitStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(0,ID_INDICATOR_SOURCE,SBPS_NORMAL,500);
	m_wndStatusBar.SetPaneText(0,_T(""));

	CString path = ((Cradar_wspApp* )AfxGetApp())->GetAppPath();
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\source.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_SOURCE,SBPS_NORMAL,220);
	m_wndStatusBar.SetPaneText(1,_T("��ѡ������Դ"));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(1,hIcon);

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\download.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_DOWNLOAD,SBPS_NORMAL,100);
	m_wndStatusBar.SetPaneText(2,_T("0.0 bps"));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(2,hIcon);

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\radar.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_ANGLE,SBPS_NORMAL,200);
	m_wndStatusBar.SetPaneText(3,_T("����0.0 ��λ0.0"));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(3,hIcon);

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\laser.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	m_wndStatusBar.SetPaneInfo(4,ID_INDICATOR_LASER,SBPS_NORMAL,200);
	m_wndStatusBar.SetPaneText(4,_T("���� �¶�0.0 ǿ��0.0"));
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(4,hIcon);
	
	m_wndStatusBar.SetPaneInfo(5,ID_INDICATOR_MAINPROGRESS,SBPS_STRETCH ,100);
		CRect rect;
	m_wndStatusBar.GetItemRect(5,&rect);
	m_ProgressCtrl.Create(WS_VISIBLE|WS_CHILD|PBS_SMOOTH,rect,&m_wndStatusBar,0);
	m_ProgressCtrl.SetRange(0, 100);  
	m_ProgressCtrl.SetPos(100);

	//m_bStatusBarCreate = TRUE;

	return TRUE;
}

void CMainFrame::SetCommandBarIcon()
{
	CString path = ((Cradar_wspApp* )AfxGetApp())->GetAppPath();
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\customadd_16.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_PRODUCT_ADDCUSTOMPRODUCTDISPLAYSTYLE,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\localfile_16.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_DATA_LOCALFILES,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\single.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_CAPTURE_SINGLE,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\scheduler.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_CAPTURE_SCHEDULER,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\taskeditor.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_TOOL_TASKEDITOR,CSize(32,32));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\savepath.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_OPTION_SAVE,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\advance.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_OPTION_ADVANCE,CSize(16,16));

	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\about_16.ico"),IMAGE_ICON,16,16,LR_LOADFROMFILE);
	XTPImageManager()->SetIcon(hIcon,ID_APP_ABOUT,CSize(16,16));
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnClose()
{
	SaveCommandBars(_T("CommandBars"));

	CFrameWnd::OnClose();
}

void CMainFrame::OnCustomize()
{
	// Get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars != NULL)
	{
		// Instanciate the customize dialog object.
		CXTPCustomizeSheet dlg(pCommandBars);

		// Add the keyboard page to the customize dialog.
		CXTPCustomizeKeyboardPage pageKeyboard(&dlg);
		dlg.AddPage(&pageKeyboard);
		pageKeyboard.AddCategories(IDR_MAINFRAME);

		// Add the options page to the customize dialog.
		CXTPCustomizeOptionsPage pageOptions(&dlg);
		dlg.AddPage(&pageOptions);

		// Add the commands page to the customize dialog.
		CXTPCustomizeCommandsPage* pCommands = dlg.GetCommandsPage();
		pCommands->AddCategories(IDR_MAINFRAME);

		// Use the command bar manager to initialize the
		// customize dialog.
		pCommands->InsertAllCommandsCategory();
		pCommands->InsertBuiltInMenus(IDR_MAINFRAME);
		pCommands->InsertNewMenuCategory();

		// Dispaly the dialog.
		dlg.DoModal();
	}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);
	m_wndSplitter.EnableNoSize(TRUE);
	if (!CreateTaskPanel())
		return FALSE;

	m_wndSplitter.CreateView(0, 1, /*RUNTIME_CLASS(CRadarFormView)*/pContext->m_pNewViewClass, CSize(300, 0), pContext);
	m_wndSplitter.ShowTopBorder(FALSE, 0);

	m_pAdvancedProductView = new CAdvancedProductView();
	CRect rt(0,0,0,0);
	m_pAdvancedProductView->Create(NULL,NULL,WS_CHILD,rt,&m_wndSplitter,IDD_ADVANCEDPRODUCTVIEW,pContext);
	m_pAdvancedProductView->OnInitialUpdate();
	m_pAdvancedProductView->ShowWindow(SW_HIDE);

	return true;
	//return CXTPFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::CreateTaskPanel()
{
	if (!m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_TABSTOP, CRect(0, 0, 0, 0), &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0)))
		return FALSE;

	m_wndTaskPanel.SetOwner(this);
	CImageList *pImageList = new CImageList();
	pImageList->Create(32, 32, ILC_COLOR32, 5, 5);

	CString path = ((Cradar_wspApp* )AfxGetApp())->GetAppPath();
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\product_32.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\custom_32.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\datasource_32.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\capture.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\options_32.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\tools_32.ico"),IMAGE_ICON,32,32,LR_LOADFROMFILE);
	pImageList->Add(hIcon);
	m_wndTaskPanel.SetImageList(pImageList,CSize(32,32));

	m_wndSplitter.SetColumnInfo(0, 250, 0);

	CreateTaskPanelItems();

	return TRUE;
}

void CMainFrame::CreateTaskPanelItems()
{
	CXTPTaskPanelGroup* pGroupBaseProducts = m_wndTaskPanel.AddGroup(IDS_BASE_PRODUCTS);
	pGroupBaseProducts->SetIconIndex(0);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_SIGNAL_1ST, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_SIGNAL_3RD, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_SIGNAL_5TH, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_EXTINCTIONCOEFFICIENT_1ST, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_EXTINCTIONCOEFFICIENT_3RD, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_EXTINCTIONCOEFFICIENT_5TH, -1);
	pGroupBaseProducts->AddLinkItem(ID_PRODUCT_DEPOLAR_RATIO, -1);
	pGroupBaseProducts->SetSpecialGroup();

	CXTPTaskPanelGroup* pGroupCustomDislpay = m_wndTaskPanel.AddGroup(IDS_CUSTOM_DISPLAY);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_OPTICDEPTH, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_ATMOSBOUNDARY, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_CLOUD_1ST, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_CLOUD_3RD, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_CLOUD_5TH, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_VISIBLE, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_MIXHEIGHT, -1);
	pGroupCustomDislpay->AddLinkItem(ID_PRODUCT_MASSCONCENTRATION, -1);
	pGroupCustomDislpay->SetIconIndex(1);

	CXTPTaskPanelGroup* pGroupDataSrc= m_wndTaskPanel.AddGroup(IDS_DATA_SRC);
	pGroupDataSrc->SetIconIndex(2);
	pGroupDataSrc->AddLinkItem(ID_DATA_LOCALFILES,-1);
	
	CXTPTaskPanelGroup* pGroupCaptureSrc= m_wndTaskPanel.AddGroup(IDS_CAPTURE);
	pGroupCaptureSrc->SetIconIndex(3);
	pGroupCaptureSrc->AddLinkItem(ID_CAPTURE_SINGLE,-1);
	pGroupCaptureSrc->AddLinkItem(ID_CAPTURE_SCHEDULER,-1);

	CXTPTaskPanelGroup* pGroupOptions= m_wndTaskPanel.AddGroup(IDS_OPTIONS);
	pGroupOptions->SetIconIndex(4);
	pGroupOptions->AddLinkItem(ID_OPTION_SAVE,-1);
	pGroupOptions->AddLinkItem(ID_OPTION_ADVANCE,-1);

	CXTPTaskPanelGroup* pGroupTools= m_wndTaskPanel.AddGroup(IDS_TOOLS);
	pGroupTools->SetIconIndex(5);
	pGroupTools->AddLinkItem(ID_TOOL_TASKEDITOR,-1);
	pGroupTools->AddLinkItem(ID_TOOL_DATATRANSFER,-1);
}

LRESULT CMainFrame::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));

	switch(wParam) {
	case XTP_TPN_CLICK:
		{
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			TRACE(_T("Click Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
			switch(pItem->GetID())
			{
			case ID_PRODUCT_SIGNAL_1ST:
				OnProductSignal1st();
				break;
			case ID_PRODUCT_SIGNAL_3RD:
				OnProductSignal3rd();
				break;
			case ID_PRODUCT_SIGNAL_5TH:
				OnProductSignal5th();
				break;
			case ID_PRODUCT_EXTINCTIONCOEFFICIENT_1ST:
				OnProductExtinctioncoefficient1st();
				break;
			case ID_PRODUCT_EXTINCTIONCOEFFICIENT_3RD:
				OnProductExtinctioncoefficient3rd();
				break;
			case ID_PRODUCT_EXTINCTIONCOEFFICIENT_5TH:
				OnProductExtinctioncoefficient5th();
				break;
			case ID_PRODUCT_DEPOLAR_RATIO:
				OnProductDepolarRatio();
				break;
			case ID_PRODUCT_OPTICDEPTH:
				OnProductOpticdepth();
				break;
			case ID_PRODUCT_ATMOSBOUNDARY:
				OnProductAtmosboundary();
				break;
			case ID_PRODUCT_CLOUD_1ST:
				OnProductCloud1st();
				break;
			case ID_PRODUCT_CLOUD_3RD:
				OnProductCloud3rd();
				break;
			case ID_PRODUCT_CLOUD_5TH:
				OnProductCloud5th();
				break;
			case ID_PRODUCT_VISIBLE:
				OnProductVisible();
				break;
			case ID_PRODUCT_MIXHEIGHT:
				OnProductMixheight();
				break;
			case ID_PRODUCT_MASSCONCENTRATION:
				OnProductMassconcentration();
				break;
			case ID_DATA_LOCALFILES:
				OnDataLocalfiles();
				break;
			case ID_CAPTURE_SINGLE:
				OnCaptureSingle();
				break;
			case ID_CAPTURE_SCHEDULER:
				OnCaptureScheduler();
				break;
			case ID_OPTION_SAVE:
				OnOptionSave();
				break;
			case ID_OPTION_ADVANCE:
				OnOptionAdvance();
				break;
			case ID_TOOL_TASKEDITOR:
				OnToolTaskeditor();
				break;
			default:
				break;
			}
		}
		break;

	case XTP_TPN_RCLICK:
		{
			CXTPTaskPanelItem* pItem = (CXTPTaskPanelItem*)lParam;
			TRACE(_T("RClick Event: pItem.Caption = %s, pItem.ID = %i\n"), pItem->GetCaption(), pItem->GetID());
		}
		break;
	}

	return 0;
}

void CMainFrame::OnProductSignal1st()
{	
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_OrginSignal,1);
}

void CMainFrame::OnProductSignal3rd()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_OrginSignal,3);
}

void CMainFrame::OnProductSignal5th()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_OrginSignal,5);
}

void CMainFrame::OnProductExtinctioncoefficient1st()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_ExtinctionCoefficient,1);
}

void CMainFrame::OnProductExtinctioncoefficient3rd()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_ExtinctionCoefficient,3);	
}

void CMainFrame::OnProductExtinctioncoefficient5th()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_ExtinctionCoefficient,5);	
}

void CMainFrame::OnProductDepolarRatio()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_DepolarizationRatio);
}

void CMainFrame::OnDataLocalfiles()
{
	CDataBrowserDlg dlg;

	if(dlg.DoModal() != IDOK)
		return;

	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->OpenRecords(dlg.m_pDatRecords);
		
	m_wndStatusBar.SetPaneText(1,_T("��ʷ����"));
}

LRESULT CMainFrame::OnCaptureEvent(WPARAM wParam,LPARAM lParam)
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));

	if(wParam == 1)//�����ɼ�
	{
		if(lParam == 0)
		{
			if(!pDoc->m_bRTAcquire)
				return 0;

			//CDatRecord *pRecord = new CDatRecord(); 
			CSample *pSample/* pRecord->m_pSamples */ = new CSample(pDoc->m_AcquireHelper.GetChannelNum(),
												pDoc->m_AcquireHelper.GetChannelDistCount(1),TRUE);
			
			LicelAcq_GetData(pSample->m_pData,pDoc->m_AcquireHelper.GetChannelDistCount(1));
			pDoc->AddRealTimeRecord2Model(pSample/* pRecord */);

			pRecord->Clear();
			delete pRecord;
		}
	}

	return 0;
}

void CMainFrame::UpdateWorkFlowState(int nIDString,int pos)
{
	if(nIDString == 0)
	{
		if(pos)
			m_ProgressCtrl.SetRange(0,pos);
		else m_ProgressCtrl.SetPos(0);
	}
	else
	{
		m_ProgressCtrl.SetPos(pos);
	}
}

void CMainFrame::UpdateCursorPos(CString& posString)
{
	m_wndStatusBar.SetPaneText(0,posString);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition(); 
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));

	if(nIDEvent == 1)
	{
		pDoc->StartRealTime();
		pDoc->m_AcquireHelper.OpenLicelCard(this->GetSafeHwnd());
		KillTimer(1);
		SetTimer(2,pDoc->m_AcquireHelper.GetDtScaleFactor() * 1000,NULL);
		return;
	}
	else if(nIDEvent == 2)
	{
		pDoc->RealTimeHook();
	}
	
	CXTPFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnOptionAdvance()
{
	
}

void CMainFrame::OnOptionSave()
{
	CSaveSettingsDlg dlg;
	if(dlg.DoModal() != IDOK)
		return;

	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->LoadSavePath();
}

void CMainFrame::OnCaptureSingle()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));

	CCaptureTaskDlg m_CaptureTaskDlg;
	m_CaptureTaskDlg.SetCapturePeriod(pDoc->m_AcquireHelper.GetDtScaleFactor());
	if(m_CaptureTaskDlg.DoModal() != IDOK)
		return;

	if(pDoc->m_AcquireHelper.GetDtScaleFactor() == m_CaptureTaskDlg.GetCapturePeriod())
		return;

	KillTimer(2);
	pDoc->StopRealTime();
	pDoc->StartRealTime(m_CaptureTaskDlg.GetCapturePeriod());
	SetTimer(2,m_CaptureTaskDlg.GetCapturePeriod()*1000,NULL);
}

void CMainFrame::OnCaptureScheduler()
{
	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CXTPFrameWnd::OnSize(nType, cx, cy);

	if(!m_bInitial)
		return;

	CRect rtPane;
	m_wndSplitter.GetPane(0,1)->GetWindowRect(&rtPane);
	m_wndSplitter.ScreenToClient(&rtPane);
	m_pAdvancedProductView->MoveWindow(rtPane);

	CRect rect;
	m_wndStatusBar.GetItemRect(5,&rect);
	m_ProgressCtrl.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
}

void CMainFrame::OnProductOpticdepth()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_OpticDepth);	
}

void CMainFrame::OnProductCloud1st()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_Cloud,1);	
}

void CMainFrame::OnProductCloud3rd()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_Cloud,3);	
}

void CMainFrame::OnProductCloud5th()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_Cloud,5);	
}

void CMainFrame::OnProductAtmosboundary()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_AtmosphereBoundary);	
}

void CMainFrame::OnProductVisible()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_Visiblity,0);	
}

void CMainFrame::OnProductMixheight()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_MixHeight);		
}

void CMainFrame::OnProductMassconcentration()
{
	POSITION ps = ((Cradar_wspApp*)AfxGetApp())->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate = ((Cradar_wspApp*)AfxGetApp())->GetNextDocTemplate(ps);
	ps = pDocTemplate->GetFirstDocPosition();
	CRadar_wspDoc *pDoc = (CRadar_wspDoc *)(pDocTemplate->GetNextDoc(ps));
	pDoc->AddProduct(CProductHelper::ProductType_MassConcentration);	
}

void CMainFrame::OnToolTaskeditor()
{
	CTaskEditDlg dlg;
	dlg.DoModal();
}