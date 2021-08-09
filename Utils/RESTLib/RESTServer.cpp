#include "StdAfx.h"
#include "RESTServer.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace web::json;

namespace
{
	enum class RequestMethodID
	{
		GET,
		POST,
		PUT,
		DEL,
	};

	const std::map<const utility::string_t, const RequestMethodID> c_methodMap =
	{
		{ L"GET",  RequestMethodID::GET  },
		{ L"POST", RequestMethodID::POST },
		{ L"PUT",  RequestMethodID::PUT  },
		{ L"DEL",  RequestMethodID::DEL  }
	};
}

RESTServer::RESTServer(TRESTCtxPtr pCtx)
	: m_pListener(nullptr)
	, m_pCtx(pCtx)
	, m_transactionCounter(0)
{
	// initialize the URL to some defaults
	ServerURI URI{ "http", "localhost", 8080, "api" };
	updateURI(URI);
}

RESTServer::RESTServer(const RESTServer& other)
	: m_pListener(other.m_pListener)
	, m_pCtx(other.m_pCtx)
	, m_transactionCounter(other.m_transactionCounter)
{
}

RESTServer& RESTServer::operator=(const RESTServer& other)
{
	m_pListener  = other.m_pListener;
	m_pCtx       = other.m_pCtx;
	m_transactionCounter = other.m_transactionCounter;

	return *this;
}

RESTServer::~RESTServer()
{
	stopServer();
}

void RESTServer::addEndpoint(std::string path, TEndpointPtr pEndpoint)
{
	if (!pEndpoint) return;

	TLock lock(m_mutex);
	m_endpoints.insert_or_assign(path, pEndpoint);
	m_pCtx->addEndpoint(path);
}

bool RESTServer::startServer()
{
	try
	{
		auto info = m_pCtx->serverInfo();
		if (info.URLString.empty())
		{
			// we can't start the server without a URL
			return false;
		}
		
		// kill the current listener if there is one
		stopServer();
		 
		auto URL = utility::conversions::to_string_t(getURL());

		// create and open the listener
		auto pListener = std::make_shared<http_listener>(URL);
		
		pListener->support([&](http_request req) {
			handleRequest(req);
		});
		pListener->open().wait();
		
		// update the listener pointer
		setListener(pListener);
		
		// update the server status and metadata
		setStatus(ServerInfo::ServerStatus::Listening);

		return true;
	}
	catch(std::exception & err)
	{
		std::string reason = "Failed to start server: ";
		reason.append(err.what());
		throw std::runtime_error(reason);
	}
}

void RESTServer::stopServer()
{
	TLock lock(m_mutex);
	if (m_pListener)
	{
		// kill the current listener if there is one
		m_pListener->close().wait();
		m_pListener = nullptr;
	}
	auto& info = m_pCtx->serverInfo(); 
	info.serverState = ServerInfo::ServerStatus::Uninitialized;
}

void RESTServer::handleRequest(http_request req)
{
	{
		// increment the transaction counter
		TLock lock(m_mutex);
		m_transactionCounter++;
	}
	
	// determine which endpoint we want and retrieve a pointer to it
	utility::string_t endpointName = req.relative_uri().path();
	auto pEndpoint = retrieveEndpoint(utility::conversions::to_utf8string(endpointName));
 
	if (!pEndpoint)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody("Endpoint not found");
		req.reply(status_codes::BadRequest, errBody);
		return;
	}

	// retrieve the method from the request and grab its ID reflection
	auto mthd = req.method();
	if (c_methodMap.find(mthd) == c_methodMap.end())
	{
		auto errBody = RESTEndpoint::createErrorJsonBody("Method reflection not found");
		req.reply(status_codes::InternalError, errBody);
		return;
	}
	
	// call the appropriate method handler for that endpoint
	auto m = c_methodMap.at(mthd);
	switch (m)
	{
	case RequestMethodID::GET:
		pEndpoint->handleGet(req, m_pCtx);
		break;
	case RequestMethodID::POST:
		pEndpoint->handlePost(req, m_pCtx);
		break;
	case RequestMethodID::PUT:
		pEndpoint->handlePut(req, m_pCtx);
		break;
	case RequestMethodID::DEL:
		pEndpoint->handleDelete(req, m_pCtx);
		break;
	default:
		auto errBody = RESTEndpoint::createErrorJsonBody("Method not recognized");
		req.reply(status_codes::BadRequest, errBody);
		break;
	}	
}

TEndpointPtr RESTServer::retrieveEndpoint(const std::string name) const
{
	TLock lock(m_mutex);
	// look up our endpoint in the map using the string from the request
	TEndpointPtr pEndpoint = nullptr;
	auto endpointData = m_endpoints.find(name);

	if (endpointData != m_endpoints.end())
	{
		pEndpoint = endpointData->second;
	}

	return pEndpoint;
}

void RESTServer::updateURI(const ServerURI& meta)
{
	TLock lock(m_mutex);
	auto& serverInfo = m_pCtx->serverInfo();

	serverInfo.URI.schema = meta.schema;
	serverInfo.URI.host = meta.host;
	serverInfo.URI.port = meta.port;
	serverInfo.URI.root = meta.root;

	// construct the URL fromthe metadata
	std::ostringstream oss;
	oss << meta.schema << "://" << meta.host << ":" << meta.port << "/" << meta.root;
	serverInfo.URLString = oss.str();
}

