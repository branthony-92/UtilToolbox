#include "StdAfx.h"
#include "RESTServer.h"
#include "HTTPUtils.h"


Server::Server(TRESTCtxPtr pCtx, unsigned int threads)
	: m_pListener(nullptr)
	, m_pIOContext(nullptr)
	, m_pSSLContext(nullptr)
	, m_threadCount(threads)
	, m_ioCtxThreads()
	, m_pCtx(pCtx)
{
	// make sure we have at least 1 thread
	m_threadCount = std::max(1u, m_threadCount);
	m_ioCtxThreads.reserve(m_threadCount);
}

void Server::startServer(std::string address, unsigned short port, SessionPtr pSessionPrototype, const SSLCtxInitHandler& sslInitHandler)
{
	reset();
	m_pIOContext = std::make_shared<net::io_context>(m_threadCount);
	m_pSSLContext = std::make_shared<ssl::context>(ssl::context::tlsv12_server);

	std::string schema = "http";
	// server-owner defined SSL certificate initialization
	if (pSessionPrototype->m_useSSLTLS)
	{
		schema = "https";
		sslInitHandler(*m_pSSLContext);
	}

	m_pListener = std::make_shared<Listener>(
		*m_pIOContext,
		*m_pSSLContext,
		tcp::endpoint{ net::ip::make_address(address), port },
		pSessionPrototype);

	auto pServerInfo = m_pCtx->getServerInfo();
	pServerInfo->getURI()->setAddress(address);
	pServerInfo->getURI()->setPort(port);
	pServerInfo->setAPIVer(1.0);
	pServerInfo->setIdleTimeout(600);

	m_pListener->run();

	for (auto i = 0u; i < m_threadCount; i++)
	{
		// run our IO context threads
		m_ioCtxThreads.emplace_back([&] { m_pIOContext->run(); });
	}

	auto name = m_pCtx->getName();
	if (name.empty()) name = "Test Server";

	std::cout << name << " Listening: " << schema <<"://" << address << ":" << port <<"\n";
}

bool Server::reset()
{
	try
	{
		m_pListener = nullptr;
		m_pIOContext = nullptr;
		m_pSSLContext = nullptr;
		m_ioCtxThreads.clear();
		return true;
	}
	catch (std::exception /*& err*/)
	{
		return false;
	}
}

void Server::shutdown()
{
	// shutdown handling logic goes here
}


void Server::addEndpoint(std::string path, ReqHandlerPtr pEndpoint)
{
	if (!pEndpoint) return;

	TLock lock(m_mutex);
	m_endpoints.insert_or_assign(path, pEndpoint);
	m_pCtx->addEndpoint(path);
}