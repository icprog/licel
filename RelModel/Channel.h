#pragma once
#include "ChannelProperty.h"
#include "Sample.h"

class __declspec(dllexport) CChannel
{
public:
	CChannel();
	~CChannel(void);

public:
	CChannelProperty* m_pChannelProperty;
    CSample* m_pSample;
};