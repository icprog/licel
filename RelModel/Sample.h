#pragma once

/*!
 * @class CSample
 * @brief PiePlot�ؼ���
 *
 *        PiePlot�ؼ��࣬�̳���CWnd���Ǽ������µ�α��ͼ���ٲ�ͼ�ؼ���
 *        ��һ�����ݰ��սǶȽ��л��ƣ����Ʋ��õ�
 *        ��ɫ�ǽ����ݵ�ֵ��Ϊlevelֵͨ��CLevelBar��
 *        ��ȡ��Ӧ����ɫ��
 */

class __declspec(dllexport) CSample
{
public:
	CSample(void);
	CSample(unsigned int col,BOOL bAllocated);
	CSample(unsigned int col,unsigned int row,BOOL bAllocated);
	~CSample(void);

	const static unsigned int MAX_SAMPLE_COL = 8;
	
protected:
	unsigned int m_Row;//ʵ��ʹ�ô�С
	unsigned int m_Col;
	unsigned int m_AllocateRow;//�����С
	
public:
	double* m_pData[MAX_SAMPLE_COL];
	int GetLength() { return m_Row; }//��ȡʵ��ʹ�ó���
	int GetCol() { return m_Col; }
	void Clear();
	
	inline void Add(double *v,int cols);
	void Acc(double *v[],int col,int row);
	void Acc(CSample* pSample);
	void Average(int factor);
	void CopyFrom(CSample *pSample);
	void Zero();

protected:
	void IncreaseSize(unsigned int size);
};

