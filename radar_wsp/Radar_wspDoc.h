// radar_wspDoc.h : interface of the Cradar_wspDoc class
//
#pragma once
#include "RadarFormView.h"
#include "CaptureTaskDlg.h"
#include "AxisConfigHelper.h"
#include <vector>
using namespace std;

class CRadar_wspDoc : public CDocument
{
protected:
	CRadar_wspDoc();
	DECLARE_DYNCREATE(CRadar_wspDoc)

public:
	typedef enum tagProductPageType
	{
		ProductPage_Simple = 0,
		ProductPage_Advanced
	}ProductPageType;

	typedef enum tagWorkMode
	{
		WorkMode_RT = 0,
		WorkMode_FILE
	}WorkMode;

	CString m_SavePath;
	BOOL m_bSaveEnable;
	WorkMode m_WorkMode;

	CAcquireHelper m_AcquireHelper;
	CProductHelper m_ProductHelper;
	CAxisConfigHelper m_AxisConfigHelper;

protected:
	ProductPageType m_curProductPageType;
	CDatRecord* m_pCurrRecord;

protected:
	void CreateFormatFactory();
	void CloseFormatFactory();
	void SaveDatFile(CDatRecord *pRecord);
	void ProductViewSwitch(ProductPageType type);

public:
	void ProductViewSwitch();

public:
	void LoadSavePath();
	BOOL AddProduct(CProductHelper::ProductType product,CProductHelper::ProductChannelType ch=CProductHelper::ProductChannelType_1ch);
	BOOL OpenRecords(vector<CDatRecord *> *pRecordList);

	void StartRealTime(unsigned int period);
	void StartRealTime();
	void RealTimeHook();
	void StopRealTime();
	void AddRealTimeRecord2Model(CDatRecord *pRecord);
	
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

public:
	virtual ~CRadar_wspDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};
