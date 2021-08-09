#include "StdAfx.h"
#include "RESTEndpointContext.h"

bool RESTEndpointContext::addEndpoint(std::string name)
{
	if (m_endpoints.count(name) > 0) return false;

	m_endpoints.insert(name);
	return true;
}

bool RESTEndpointContext::stopRequested()
{
	bool val = m_stopFlag.load();
	
	// consume the flag on retrieval
	if (val) m_stopFlag = false;

	return val;
}

bool RESTEndpointContext::resetRequested()
{
	bool val = m_resetFlag.load();

	// consume the flag on retrieval
	if (val) m_resetFlag = false;

	return val;
}
