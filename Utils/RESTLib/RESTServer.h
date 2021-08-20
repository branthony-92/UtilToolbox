#ifndef RESTSERVER_H
#define RESTSERVER_H

#include "MdlUriInfo.h"
#include "MdlServerInfo.h"
#include "MdlResponseInfo.h"
#include "MdlErrorInfo.h"

#include "RESTEndpoint.h"
#include "RESTServerContext.h"
#include "Listener.h"

typedef std::lock_guard<std::mutex> TLock;

/*
	This is the actual server class that will create and own the listener
*/
class Server
{
public:
	using SSLCtxInitHandler = std::function<void(boost::asio::ssl::context&)>;
	using TEndpointMap = std::map<std::string, ReqHandlerPtr>;
	using TEndpointMapData = std::pair<std::string, ReqHandlerPtr>;

	Server(TRESTCtxPtr pCtx, unsigned int threads = 1);
	virtual ~Server() {}

	virtual void startServer(std::string address, unsigned short port, SessionPtr pSessionPrototype, const SSLCtxInitHandler& sslInitHandler);
	virtual bool reset();
	void shutdown();

	void addEndpoint(std::string path, ReqHandlerPtr pEndpoint);
	ServerInfoBody::ServerStatus getStatus() const      { TLock lock(m_mutex); return m_pCtx->getServerInfo()->getState(); }
	void setStatus(ServerInfoBody::ServerStatus status) { TLock lock(m_mutex); m_pCtx->getServerInfo()->setState(status); }
	std::string getURL() const { TLock lock(m_mutex); return m_pCtx->getServerInfo()->getURLString(); }
	std::shared_ptr<ServerInfoBody> getServerInfo() const { TLock lock(m_mutex); return m_pCtx->getServerInfo(); }

protected:
	ListenerPtr    m_pListener;
	TEndpointMap m_endpoints;

	TRESTCtxPtr  m_pCtx;
	IOCtxPtr     m_pIOContext;
	SSLCtxPtr    m_pSSLContext;
	unsigned int m_threadCount;
	std::vector<std::thread> m_ioCtxThreads;
	mutable std::mutex m_mutex;
};
typedef std::shared_ptr<Server> ServerPtr;

#endif // !RESTSERVER_H