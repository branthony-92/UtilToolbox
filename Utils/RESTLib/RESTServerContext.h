#ifndef RESTENDPOINTCONTEXT_H
#define RESTENDPOINTCONTEXT_H

#include "Models.h"

class RESTServerContext
{
protected:

	std::string m_name;

	ServerInfo m_serverInfo;

	std::chrono::system_clock::time_point m_lastTransaction;

	std::atomic_bool m_stopFlag;
	std::atomic_bool m_resetFlag;
	TConnectionMgrPtr     m_pConnectionMgr;

public:
	RESTServerContext(std::string name) 
		: m_name(name)
		, m_lastTransaction()
		, m_stopFlag(false)
		, m_resetFlag(false)
		, m_serverInfo()
		, m_pConnectionMgr(std::make_unique<ConnectionManager>())
	{}

	ServerInfo& serverInfo() { return m_serverInfo; }

	virtual ~RESTServerContext() {}

	// endpoint management
	bool addEndpoint(std::string name);
	std::set<std::string> getEndpoints() const { return m_serverInfo.endpoints; }
	
	// connection management
	ConnectionInfo handleConnectionRequest(ConnectionManager::TokenType type, unsigned int timeout);
	ConnectionInfo handleConnectionRefreshRequest(unsigned int id);
	ConnectionInfo getConnectionInfo(unsigned int id);

	bool checkConnection(unsigned int id, std::string token);

	std::string getName() const { return m_name; }

	void stop()  { m_stopFlag = true; }
	void reset() { m_resetFlag = true; }

	bool stopRequested();
	bool resetRequested();

	void checkTimeout();
	void ping();
};
typedef std::shared_ptr<RESTServerContext> TRESTCtxPtr;

#endif // !RESTENDPOINTCONTEXT_H
