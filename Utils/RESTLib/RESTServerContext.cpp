#include "StdAfx.h"
#include "RESTServerContext.h"
#include "Listener.h"

RESTServerContext::RESTServerContext(std::string name)
	: m_name(name)
	, m_lastTransaction()
	, m_transactionID(0u)
	, m_stopFlag(false)
	, m_resetFlag(false)
	, m_pServerInfo(std::make_shared<ServerInfoBody>())
	, m_pConnectionMgr(std::make_unique<ConnectionManager>())
{
}

void RESTServerContext::registerHandler(std::string path, ReqHandlerPtr pEndpoint)
{
	if (!pEndpoint) return;
	m_requestHandlers.insert_or_assign(path, pEndpoint);
	addEndpoint(path);
}

std::shared_ptr<HTTPRequestHandler> RESTServerContext::retrieveHandler(std::string name)
{
	auto handler = m_requestHandlers.find(name);
	if (handler == m_requestHandlers.end()) return nullptr;
	return handler->second;
}

bool RESTServerContext::hasHandlers() const
{
	return !m_requestHandlers.empty();
}

bool RESTServerContext::addEndpoint(std::string name)
{
	if (!m_pServerInfo) return false;

	auto endpoints = m_pServerInfo->getEndpointNames();

	if (endpoints.count(name) > 0) return false;

	endpoints.insert(name);
	m_pServerInfo->setEndpointNames(endpoints);
	return true;
}

std::string RESTServerContext::findResource(std::string target)
{
	if (target == "/")          return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\index.html";
	else if (target == "/test") return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\Test\\test.html";
	else if (target == "/img")  return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\Test\\img.jpg";
	return "";
}

std::shared_ptr<ConnectionInfoBody> RESTServerContext::handleConnectionRequest(TokenInfoBody::Lifetime type, unsigned int timeout)
{
	auto pConnection = m_pConnectionMgr->openNewConnection(type, timeout);

	if (!pConnection)
	{
		return nullptr;
	}
	auto pInfo = std::make_shared<ConnectionInfoBody>();

	// The client will get these values in return  and will need them 
	// for all future requests
	pInfo->setConnectionID(pConnection->getID());
	pInfo->setToken(pConnection->getToken());

	return pInfo;
}

std::shared_ptr<ConnectionInfoBody> RESTServerContext::handleConnectionRefreshRequest(unsigned int id)
{
	auto pToken = m_pConnectionMgr->refreshConnection(id);

	if (!pToken) return nullptr;

	auto pInfo = std::make_shared<ConnectionInfoBody>();
	pInfo->setConnectionID(id);
	pInfo->setToken(pToken);

	return pInfo;
}

std::shared_ptr<ConnectionInfoBody> RESTServerContext::getConnectionInfo(unsigned int id)
{
	auto pConnection = m_pConnectionMgr->getConnection(id);

	if (!pConnection)
	{
		return nullptr;
	}
	auto pToken = pConnection->getToken();
	if (!pToken) return nullptr;

	auto pInfo = std::make_shared<ConnectionInfoBody>();
	pInfo->setConnectionID(id);
	pInfo->setToken(pToken);

	return pInfo;
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

	if (dt > m_pServerInfo->getIdleTimeout())
	{
		// server has been idle too long, signal a stop
		stop();
	}
}

void RESTServerContext::ping()
{
	m_lastTransaction = std::chrono::system_clock::now();
}

unsigned int RESTServerContext::getNextransactionID()
{
	return m_transactionID++;
}
