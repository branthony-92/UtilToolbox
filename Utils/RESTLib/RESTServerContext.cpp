#include "StdAfx.h"
#include "RESTServerContext.h"

bool RESTServerContext::addEndpoint(std::string name)
{
	if (m_endpoints.count(name) > 0) return false;

	m_endpoints.insert(name);
	return true;
}

ConnectionInfo RESTServerContext::handleConnectionRequest(ConnectionManager::TokenType type, unsigned int timeout)
{
	ConnectionInfo info;
	auto pConnection = m_pConnectionMgr->openNewConnection(type, timeout);

	if (!pConnection)
	{
		return info;
	}

	// The client will get these values in return  and will need them 
	// for all future requests
	info.id    = pConnection->getID();
	info.token = pConnection->getToken();

	return info;
}

ConnectionInfo RESTServerContext::handleConnectionRefreshRequest(unsigned int id)
{
	auto token = m_pConnectionMgr->refreshConnection(id);

	ConnectionInfo info;
	info.id    = id;
	info.token = token;

	return info;
}

ConnectionInfo RESTServerContext::getConnectionInfo(unsigned int id)
{
	auto pConnection = m_pConnectionMgr->getConnection(id);

	if (!pConnection)
	{
		return ConnectionInfo();
	}
	auto token = pConnection->getToken();

	ConnectionInfo info;
	info.id = id;
	info.token = token;

	return info;
}

bool RESTServerContext::checkConnection(unsigned int id, std::string token)
{
	auto status = m_pConnectionMgr->validateConnection(id, token);
	auto valid = false;
	switch (status)
	{
	case ConnectionManager::ConnectionStatus::Open:
		// conneciton is fine
		valid = true;
		break;
	case ConnectionManager::ConnectionStatus::Uninitialized:
	case ConnectionManager::ConnectionStatus::StaleToken:
	case ConnectionManager::ConnectionStatus::BadToken:
	case ConnectionManager::ConnectionStatus::Closed:
	case ConnectionManager::ConnectionStatus::Invalid:
	default:
		break;
	}
	return valid;
}

bool RESTServerContext::stopRequested()
{
	bool val = m_stopFlag.load();
	
	// consume the flag on retrieval
	if (val) m_stopFlag = false;

	return val;
}

bool RESTServerContext::resetRequested()
{
	bool val = m_resetFlag.load();

	// consume the flag on retrieval
	if (val) m_resetFlag = false;

	return val;
}

void RESTServerContext::checkTimeout()
{
	auto now = std::chrono::system_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::seconds>(now - m_lastTransaction).count();

	if (dt > m_serverInfo.serverIdleTimoutSec)
	{
		// server has been idle too long, signal a stop
		stop();
	}
}

void RESTServerContext::ping()
{
	m_lastTransaction = std::chrono::system_clock::now();
}
