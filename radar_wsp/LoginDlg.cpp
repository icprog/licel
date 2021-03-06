// LoginDlg.cpp
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include <vector>
using namespace std;

// CLoginDlg

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_bLogin = FALSE;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CLoginDlg


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString path = ((Cradar_wspApp* )AfxGetApp())->GetAppPath();
	HBITMAP hLogCaption = (HBITMAP)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\login_cover.bmp"),IMAGE_BITMAP,440,107,LR_LOADFROMFILE);
	((CStatic *)GetDlgItem(IDC_STATIC_CAPTION))->SetBitmap(hLogCaption);

	HICON hLogin =  (HICON)LoadImage(AfxGetInstanceHandle(),path + _T("resource\\login.ico"),IMAGE_ICON,128,128,LR_LOADFROMFILE);
	((CStatic *)GetDlgItem(IDC_STATIC_LOGIN))->SetIcon(hLogin);

	GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowTextW(_T(""));

	return TRUE; 
}

void CLoginDlg::OnBnClickedButtonLogin()
{
	GetDlgItem(IDC_EDIT_LOGINUSR)->GetWindowText(m_UsrName);
	CString pwdString;
	GetDlgItem(IDC_EDIT_LOGINPWD)->GetWindowText(pwdString);
	if(((CButton*)GetDlgItem(IDC_CHECK_DBUSR))->GetCheck() == BST_CHECKED)
	{
		
		if(m_UsrName == _T(""))
		{
			GetDlgItem(IDC_STATIC_LOGININFO)->SetWindowTextW(_T("请输入用户名!"));
			return;
		}
		else
		{
			m_bLogin = TRUE;
			OnOK();
		}
	}
}
