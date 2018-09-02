#pragma once

/*!
 * @class CSample
 * @brief PiePlot控件类
 *
 *        PiePlot控件类，继承自CWnd，是极坐标下的伪彩图或瀑布图控件，
 *        对一组数据按照角度进行绘制，绘制采用的
 *        颜色是将数据的值作为level值通过CLevelBar类
 *        获取对应的颜色。
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
	unsigned int m_Row;//实际使用大小
	unsigned int m_Col;
	unsigned int m_AllocateRow;//分配大小
	
public:
	double* m_pData[MAX_SAMPLE_COL];
	int GetLength() { return m_Row; }//获取实际使用长度
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

