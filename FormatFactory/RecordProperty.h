#pragma once

#include <fstream>
using namespace std;

class AFX_EXT_CLASS CRecordProperty
{
public:
	CRecordProperty();
	~CRecordProperty();
	
	//̽��ģʽ
    typedef enum tagModeType
    {
        ModeType_Curve = 1,//����̽��
        ModeType_Sum
    }ModeType;

public:
	//̽��ģʽ
	ModeType m_ModeType;
    unsigned int m_sTotalSeconds;//��ʼ�����
	unsigned int m_eTotalSeconds;//��������� 
	unsigned short m_TotalDays;//������
	double m_FyAngle;//������ 
	double m_FwAngle;//��λ��

public:
    void SetCurrForStartTime();
    void SetCurrForEndTime();
	void GetStartTime(CTime& t);
	void GetEndTime(CTime& t);

public:
	void Store(ofstream& ofp);
	void Restore(ifstream& ifp);
};