#pragma once
class CAxisConfigHelper
{
public:
	CAxisConfigHelper(void);
	~CAxisConfigHelper(void);

	typedef struct tagAxisSettings
	{
		double default_min;//信号最小值
		double default_max;//信号最大值

		double min;//信号最小值
		double max;//信号最大值
		bool log_t;//信号对数坐标

		double level_min;//色条最小值
		double level_max;//色条最大值
		bool level_visible;//色条显示选项
		bool log_c;      //色条对数坐标
	}AxisSettings,*PTR_AxisSettings;

protected:
	AxisSettings m_AxisSettings[CProductHelper::ProductChannelType_Sum][CProductHelper::ProductType_Sum];
	CString m_FilePath;

public:
	void ConfigFromFile(CString file);
	void SaveModify(CProductHelper::ProductType productType,unsigned int ch = 0);
	PTR_AxisSettings GetProductConfig(CProductHelper::ProductType productType,unsigned int ch = 0);
};

