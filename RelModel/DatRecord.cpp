#include "StdAfx.h"
#include "DatRecord.h"

CDatRecord::CDatRecord()
{
	m_pSamples = NULL;
	m_pRegular = NULL;
	m_pBackScatter = NULL;
	m_pExtinctionCoefficient = NULL;
	m_pDeplorRatio = NULL;
	m_pCloud = NULL;
	m_pOpticDepth= NULL;
	m_pBoundary = NULL;
	m_pVisiblity = NULL;
	m_pMix = NULL;
	m_pGasoloidPotency = NULL;
}

CDatRecord::~CDatRecord(void)
{
	Clear();
}


void CDatRecord::Clear()
{
	if(m_pSamples != NULL)
	{
		m_pSamples->Release();
		m_pSamples = NULL;
	}
	if(m_pRegular != NULL)
	{
		m_pRegular->Release();
		m_pRegular = NULL;
	}
	if(m_pDeplorRatio != NULL)
	{
		m_pDeplorRatio->Release();
		m_pDeplorRatio = NULL;
	}
	if(m_pBackScatter != NULL)
	{
		m_pBackScatter->Release();
		m_pBackScatter = NULL;
	}
	if(m_pExtinctionCoefficient != NULL)
	{
		m_pExtinctionCoefficient->Release();
		m_pExtinctionCoefficient = NULL;
	}
	if(m_pCloud != NULL)
	{
		m_pCloud->Release();
		m_pCloud = NULL;
	}
	if(m_pOpticDepth != NULL)
	{
		m_pOpticDepth->Release();
		m_pOpticDepth = NULL;
	}
	if(m_pBoundary != NULL)
	{
		m_pBoundary->Release();
		m_pBoundary = NULL;
	}
	if(m_pVisiblity != NULL)
	{
		m_pVisiblity->Release();
		m_pVisiblity = NULL;
	}
	if(m_pMix != NULL)
	{
		m_pMix->Release();
		m_pMix = NULL;
	}
	if(m_pGasoloidPotency != NULL)
	{
		m_pGasoloidPotency->Release();
		m_pGasoloidPotency = NULL;
	}
}