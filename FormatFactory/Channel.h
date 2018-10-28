#pragma once
#include "ChannelProperty.h"

class AFX_EXT_CLASS CChannel
{
public:
	CChannel();
	~CChannel(void);

public:
	CChannelProperty* m_pChannelProperty;
    double* m_pDatas;
};