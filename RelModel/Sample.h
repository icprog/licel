#pragma once

class __declspec(dllexport) CSample
{
public:
	CSample(void);
	CSample(unsigned int col,BOOL bAllocated);
	CSample(unsigned int col,unsigned int row,BOOL bAllocated);
	~CSample(void);

	const static unsigned int MAX_SAMPLE_COL = 8;
	
protected:
	unsigned int m_Row;//��ʹ����
	unsigned int m_Col;
	unsigned int m_AllocateRow;//������
	
public:
	double* m_pData[MAX_SAMPLE_COL];
	int GetLength() { return m_Row; }//��ȡʵ��ʹ����
	int GetCol() { return m_Col; }
	void Release();
	
	inline void Add(double *v,int cols);
	void Acc(double *v[],int col,int row);
	void Acc(CSample* pSample);
	void Average(int factor);
	void CopyFrom(CSample *pSample);
	void Zero();

protected:
	void IncreaseSize(unsigned int size);
};

