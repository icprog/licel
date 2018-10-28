// LicelSettingsPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "radar_wsp.h"
#include "LicelSettingsPage.h"
#include "afxdialogex.h"


// CLicelSettingsPage �Ի���

IMPLEMENT_DYNAMIC(CLicelSettingsPage, CXTPPropertyPage)

CLicelSettingsPage::CLicelSettingsPage()
	: CXTPPropertyPage(CLicelSettingsPage::IDD)
{

}

CLicelSettingsPage::~CLicelSettingsPage()
{
}

void CLicelSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TRIGTYPE, m_ComboTrigType);
	DDX_Control(pDX, IDC_COMBO_INPUTRANGE, m_ComboInputType);
}


BEGIN_MESSAGE_MAP(CLicelSettingsPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// CLicelSettingsPage ��Ϣ�������


BOOL CLicelSettingsPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	m_ComboTrigType.AddString(_T("�͵�ƽ����"));
	m_ComboTrigType.AddString(_T("�ߵ�ƽ����"));
	m_ComboTrigType.SetCurSel(0);

	m_ComboInputType.AddString(_T("20mV"));
	m_ComboInputType.AddString(_T("100mV"));
	m_ComboInputType.AddString(_T("500mV"));
	m_ComboInputType.SetCurSel(0);

	return TRUE;
}
