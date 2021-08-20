#ifndef RESTENDPOINTCONTEXT_H
#define RESTENDPOINTCONTEXT_H

#include "MdlServerInfo.h"
#include "MdlConnectionInfo.h"
#include "RESTConnection.h"

class HTTPRequestHandler;


class RESTServerContext
{
protected:

	std::string m_name;

	std::shared_ptr<ServerInfoBody> m_pServerInfo;

	std::chrono::system_clock::time_point m_lastTransaction;

	std::atomic_bool      m_stopFlag;
	std::atomic_bool      m_resetFlag;
	TConnectionMgrPtr     m_pConnectionMgr;

	std::map<std::string, std::shared_ptr<HTTPRequestHandler>> m_requestHandlers;

public:
	RESTServerContext(std::string name);

	std::shared_ptr<ServerInfoBody> getServerInfo() { return m_pServerInfo; }
	void setServerInfo(std::shared_ptr<ServerInfoBody> pInfo) { m_pServerInfo = pInfo; }

	virtual ~RESTServerContext() {}

	// endpoint management
	void registerHandler(std::string path, std::shared_ptr<HTTPRequestHandler> pEndpoint);
	std::shared_ptr<HTTPRequestHandler> retrieveHandler(std::string name);

	bool hasHandlers() const;

	bool addEndpoint(std::string name);
	std::set<std::string> getEndpoints() const { return m_pServerInfo->getEndpointNames(); }
	


	// connection management
	std::shared_ptr<ConnectionInfoBody> handleConnectionRequest(TokenInfoBody::Lifetime type, unsigned int timeout);
	std::shared_ptr<ConnectionInfoBody> handleConnectionRefreshRequest(unsigned int id);
	std::shared_ptr<ConnectionInfoBody> getConnectionInfo(unsigned int id);

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
