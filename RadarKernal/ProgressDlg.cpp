// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ProgressBar.SetRange(0,100);
	m_ProgressBar.SetStep(5);
	m_ProgressBar.SetPos(0);

	return TRUE;
}

void CProgressDlg::SetProgress(double percent)
{
	m_ProgressBar.SetPos(percent);
}
