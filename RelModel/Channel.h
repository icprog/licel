#pragma once
#include "ChannelProperty.h"

class __declspec(dllexport) CChannel
{
public:
	CChannel();
	~CChannel(void);

public:
	CChannelProperty* m_pChannelProperty;
    CSample* m_pSample;
};