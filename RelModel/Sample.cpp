#include "StdAfx.h"
#include "Sample.h"

CSample::CSample(void)
{
	m_Row = 0;
	m_Col = MAX_SAMPLE_COL;
	m_AllocateRow = 2000;

	for(unsigned int i=0;i<m_Col;i++)
		m_pData[i] = new double[m_AllocateRow];

	Zero();
}

CSample::CSample(unsigned int col,BOOL bAllocated)
{
	m_AllocateRow = 2000;
	m_Col = col;

	if(bAllocated)
		m_Row = m_AllocateRow;
	else m_Row = 0;

	for(unsigned int i=0;i<m_Col;i++)
		m_pData[i] = new double[m_AllocateRow];

	for(unsigned int i=m_Col;i<MAX_SAMPLE_COL;i++)
		m_pData[i] = NULL;

	Zero();
}

CSample::CSample(unsigned int col,unsigned int row,BOOL bAllocated)
{
	m_AllocateRow = row;
	m_Col = col;

	if(bAllocated)
		m_Row = m_AllocateRow;
	else m_Row = 0;

	for(unsigned int i=0;i<m_Col;i++)
		m_pData[i] = new double[m_AllocateRow];

	for(unsigned int i=m_Col;i<MAX_SAMPLE_COL;i++)
		m_pData[i] = NULL;

	Zero();
}

CSample::~CSample(void)
{
	Clear();
}

void CSample::IncreaseSize(unsigned int size)
{
	double* pTemp;
	for(unsigned int i=0;i<m_Col;i++)
	{
		pTemp = new double[m_AllocateRow + size];

		for(unsigned int j=0;j<size;j++)
			pTemp[m_AllocateRow+j] = 0;

		memcpy(pTemp,m_pData[i],m_AllocateRow*sizeof(double));
		delete[] m_pData[i];
		m_pData[i] = pTemp;
	}

	m_AllocateRow += size;
}

inline void CSample::Add(double *v,int cols)
{
	ASSERT(cols <= m_Col);

	if(m_Row == m_AllocateRow)
		IncreaseSize(m_AllocateRow>>2);

	if(cols > MAX_SAMPLE_COL)
		cols = MAX_SAMPLE_COL;

	while(cols>0)
	{
		--cols;
		m_pData[cols][m_Row] = v[cols];
	}
	m_Row++;
}

void CSample::Acc(double *v[],int col,int row)
{
	ASSERT(col <= m_Col && row <= m_AllocateRow);

	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++)
			m_pData[i][j] += v[i][j];
}

void CSample::Acc(CSample* pSample)
{
	int col = pSample->GetCol();
	int row = pSample->GetLength();

	Acc(pSample->m_pData,col,row);
}

void CSample::Average(int factor)
{
	for(int i=0;i<m_Col;i++)
		for(int j=0;j<m_Row;j++)
			m_pData[i][j] /= factor;
}

void CSample::Clear()
{
	for(unsigned int i=0;i<MAX_SAMPLE_COL;i++)
	{
		if( m_pData[i] == NULL )
			continue;

		delete[] m_pData[i];
		m_pData[i] = NULL;
	}
}

void CSample::Zero()
{
	unsigned int i,j;
	for(i=0;i<MAX_SAMPLE_COL;i++)
	{
		if( m_pData[i] == NULL )
			continue;

		for(j=0;j<m_AllocateRow;j++)
			 m_pData[i][j] = 0.0;
	}
}

void CSample::CopyFrom(CSample *pSample)
{
	if(pSample->GetLength() > m_AllocateRow)
		IncreaseSize(pSample->GetLength() - m_AllocateRow);

	m_Row = pSample->GetLength();
	for(int i=0;i<m_Col;i++)
		memcpy(m_pData[i],pSample->m_pData[i],sizeof(double)*m_Row);
}