// radar_wspDoc.h : interface of the Cradar_wspDoc class
//
#pragma once
#include "RadarFormView.h"
#include "CaptureTaskDlg.h"
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

	CString m_SavePath;
	BOOL m_bSaveEnable;
	BOOL m_bRTAcquire;

	CAcquireHelper m_AcquireHelper;
	CProductHelper m_ProductHelper;

protected:
	ProductPageType m_curProductPageType;
	CDatRecord* m_pCurrRecord;

protected:
	void SaveToDir(CDatRecord *pRecord);
	void ProductViewSwitch(ProductPageType type);
	void LoadSavePath(CString appPath);

public:
	void LoadSavePath();
	BOOL AddProduct(CProductHelper::ProductType product,unsigned int ch=0);
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
