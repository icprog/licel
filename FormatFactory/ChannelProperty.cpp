#include "StdAfx.h"
#include "ChannelProperty.h"

char* CChannelProperty::AcqTypeString[] = {"AD","PC","�ں�"};
char* CChannelProperty::WaveTypeString[] = {"ƫ��P","ƫ��S","����"};

CChannelProperty::CChannelProperty()
{

}

CChannelProperty::~CChannelProperty()
{

}

int CChannelProperty::Restore(CString config)
{
    CString idStr;
    idStr.Format(_T("channel_%d"),m_id);

    unsigned char cBuf[_MAX_PATH];
	memset(cBuf,0,_MAX_PATH);
	// ͨ������
	GetPrivateProfileString(idStr,_T("name"),_T(""),(LPWSTR)cBuf,_MAX_PATH,config);
	m_name.Empty();
	m_name.Append((LPWSTR)cBuf);

    //�ɼ�����
    unsigned short usTemp = GetPrivateProfileInt(idStr,_T("channels"),0,config);
    if(usTemp != (AcqType)usTemp)
        return -1;
    m_acqType = (AcqType)usTemp;

    // ����
    CString strTemp;
    memset(cBuf,0,_MAX_PATH);
    GetPrivateProfileString(idStr,_T("wave_len"),_T(""),(LPWSTR)cBuf,_MAX_PATH,config);
	strTemp.Empty();
	strTemp.Append((LPWSTR)cBuf);
    m_waveLen = _ttof(strTemp);

    // �ز�����
    usTemp = GetPrivateProfileInt(idStr,_T("wave_type"),0,config);
    if(usTemp != (WaveType)usTemp)
        return -2;
    m_waveType = (WaveType)usTemp;

    // // ����ֱ���
    // memset(cBuf,0,_MAX_PATH);
    // GetPrivateProfileString(idStr,_T("wave_len"),_T(""),(LPWSTR)cBuf,_MAX_PATH,configfile);
	// strTemp.Empty();
	// strTemp.Append((LPWSTR)cBuf);
    // m_distResolution = _ttof(strTemp);

    // ä���߶�
    memset(cBuf,0,_MAX_PATH);
    GetPrivateProfileString(idStr,_T("dead_zone"),_T(""),(LPWSTR)cBuf,_MAX_PATH,config);
	strTemp.Empty();
	strTemp.Append((LPWSTR)cBuf);
    m_deadZone = _ttof(strTemp);

    // �������
}

void CChannelProperty::Store(CString config)
{

}
