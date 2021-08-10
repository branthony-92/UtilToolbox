#ifndef RESTSERVER_H
#define RESTSERVER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "RESTEndpoint.h"
#include "RESTServerContext.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace web::json;

typedef std::lock_guard<std::mutex> TLock;

class RESTServer
{
public:
	RESTServer(TRESTCtxPtr pCtx);
	RESTServer(const RESTServer& other);
	RESTServer& operator=(const RESTServer& other);

	virtual ~RESTServer();

	virtual void handleRequest(http_request req);

protected:
	// Typedefs
	typedef std::shared_ptr<http_listener> TListenerPtr;
	typedef std::map<std::string, TEndpointPtr>  TEndpointMap;
	typedef std::pair<std::string, TEndpointPtr> TEndpointMapData;

	TEndpointMap m_endpoints;

	TListenerPtr m_pListener;
	TRESTCtxPtr  m_pCtx;

	unsigned int m_transactionCounter;

	mutable std::mutex m_mutex;

public:
	void addEndpoint(std::string path, TEndpointPtr pEndpoint);

	bool startServer();
	void stopServer();

	TEndpointPtr retrieveEndpoint(const std::string name) const;

	unsigned int getTransactionID() const { TLock lock(m_mutex); return m_transactionCounter; }

	ServerInfo::ServerStatus getStatus() const      { TLock lock(m_mutex); return m_pCtx->serverInfo().serverState; }
	void setStatus(ServerInfo::ServerStatus status) { TLock lock(m_mutex); m_pCtx->serverInfo().serverState = status; }

	std::string getURL() const { TLock lock(m_mutex); return m_pCtx->serverInfo().URLString; }

	TListenerPtr getListener() const         { TLock lock(m_mutex); return m_pListener; }
	void setListener(TListenerPtr pListener) { TLock lock(m_mutex);  m_pListener = pListener; }

	bool getServerListening() const { TLock lock(m_mutex); return m_pCtx->serverInfo().serverState == ServerInfo::ServerStatus::Listening; }

	void updateURI(const ServerURI& meta);
private:

};
typedef std::shared_ptr<RESTServer> TRESTServerPtr;
#endif // !RESTSERVER_H