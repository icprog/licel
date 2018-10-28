#pragma once
class CAxisConfigHelper
{
public:
	CAxisConfigHelper(void);
	~CAxisConfigHelper(void);

	typedef struct tagAxisSettings
	{
		double default_min;//�ź���Сֵ
		double default_max;//�ź����ֵ

		double min;//�ź���Сֵ
		double max;//�ź����ֵ
		bool log_t;//�źŶ�������

		double level_min;//ɫ����Сֵ
		double level_max;//ɫ�����ֵ
		bool level_visible;//ɫ����ʾѡ��
		bool log_c;      //ɫ����������
	}AxisSettings,*PTR_AxisSettings;

protected:
	AxisSettings m_AxisSettings[CProductHelper::ProductChannelType_Sum][CProductHelper::ProductType_Sum];
	CString m_FilePath;

public:
	void ConfigFromFile(CString file);
	void SaveModify(CProductHelper::ProductType productType,unsigned int ch = 0);
	PTR_AxisSettings GetProductConfig(CProductHelper::ProductType productType,unsigned int ch = 0);
};

