// RecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Radar_Wsp.h"
#include "RecordDlg.h"
#include "afxdialogex.h"


// CRecordDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDlg::IDD, pParent)
{

}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle = m_List.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
	m_List.SetExtendedStyle(dwStyle);

	return TRUE;
}

void CRecordDlg::ResetList()
{
	m_List.DeleteAllItems();
	int nCol = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCol > 0)
	{
		m_List.DeleteColumn(0);
		nCol--;
	}
}

void CRecordDlg::DisplayRecord(CProductHelper::ProductType productType,CDatRecord* pRecord)
{
	ResetList();

	unsigned int channelCount = pRecord->m_pSamples->GetCol() - 1;
	unsigned int distCount = pRecord->m_pSamples->GetLength();
	unsigned int nRow;
	CString strTemp;

	if(productType == CProductHelper::ProductType_OrginSignal)
	{
		m_List.InsertColumn(0,_T("距离库"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(1,_T("通道1"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(2,_T("通道2"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(3,_T("通道3"),LVCFMT_LEFT, 80);

		for(unsigned int i=0;i<distCount;i++)
		{
			strTemp.Format(_T("%.3f"),pRecord->m_pSamples->m_pData[0][i]);
			nRow = m_List.InsertItem(i, strTemp);
			for(unsigned int j=0;j<channelCount;j++)
			{
				strTemp.Format(_T("%.6f"),pRecord->m_pSamples->m_pData[j+1][i]);
				m_List.SetItemText(nRow,j+1,strTemp);
			}
		}
	}
	else if(productType == CProductHelper::ProductType_DepolarizationRatio)
	{
		m_List.InsertColumn(0,_T("距离库"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(1,_T("退偏振比"),LVCFMT_LEFT, 80);

		for(unsigned int j=0;j<distCount;j++)
		{
			strTemp.Format(_T("%.3f"),pRecord->m_pSamples->m_pData[0][j]);
			nRow = m_List.InsertItem(j, strTemp);

			strTemp.Format(_T("%.6f"),pRecord->m_pDeplorRatio->m_pData[1][j]);
			m_List.SetItemText(nRow,1,strTemp);
		}
	}
	else if(productType == CProductHelper::ProductType_BackScatter)
	{
		m_List.InsertColumn(0,_T("距离库"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(1,_T("通道1"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(2,_T("通道2"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(3,_T("通道3"),LVCFMT_LEFT, 80);

		for(unsigned int i=0;i<distCount;i++)
		{
			strTemp.Format(_T("%.3f"),pRecord->m_pSamples->m_pData[0][i]);
			nRow = m_List.InsertItem(i, strTemp);
			for(unsigned int j=0;j<channelCount;j++)
			{
				strTemp.Format(_T("%.6f"),pRecord->m_pBackScatter->m_pData[j+1][i]);
				m_List.SetItemText(nRow,j+1,strTemp);
			}
		}
	}
	else if(productType == CProductHelper::ProductType_ExtinctionCoefficient)
	{
		m_List.InsertColumn(0,_T("距离库"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(1,_T("通道1"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(2,_T("通道2"),LVCFMT_LEFT, 80);
		m_List.InsertColumn(3,_T("通道3"),LVCFMT_LEFT, 80);

		for(unsigned int i=0;i<distCount;i++)
		{
			strTemp.Format(_T("%.3f"),pRecord->m_pSamples->m_pData[0][i]);
			nRow = m_List.InsertItem(i, strTemp);
			for(unsigned int j=0;j<channelCount;j++)
			{
				strTemp.Format(_T("%.6f"),pRecord->m_pExtinctionCoefficient->m_pData[j+1][i]);
				m_List.SetItemText(nRow,j+1,strTemp);
			}
		}
	}
	else if(productType == CProductHelper::ProductType_Cloud)
	{
		
	}
	else if(productType == CProductHelper::ProductType_OpticDepth)
	{
		
	}
	else if(productType == CProductHelper::ProductType_AtmosphereBoundary)
	{

	}
	else if(productType == CProductHelper::ProductType_Visiblity)
	{

	}
	else if(productType == CProductHelper::ProductType_MixHeight)
	{

	}
	else if(productType == CProductHelper::ProductType_MassConcentration)
	{

	}
}

void CRecordDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);
	ResetList();
	DestroyWindow();
}

void CRecordDlg::PostNcDestroy()
{
	delete this;

	CDialogEx::PostNcDestroy();
}
