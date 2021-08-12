#include "StdAfx.h"
#include "RESTServer.h"
#include "HTTPUtils.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace web::json;
using namespace web;

namespace net = boost::asio;
namespace ssl = net::ssl;


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

bool RESTServer::startServer(utility::string_t URL)
{
	auto uri = m_pCtx->serverInfo().URI;

	stopServer();
	TListenerPtr pListener = nullptr;

	if (uri.schema != "http") return false;

	// create and open the listener (No SSL)
	pListener = std::make_shared<http_listener>(URL);

	setListener(pListener);

	return startServerInternal();
}
bool RESTServer::startServer_s(utility::string_t URL, const std::function<void(boost::asio::ssl::context&)>& ssl_context_callback)
{
	auto uri = m_pCtx->serverInfo().URI;

	stopServer();
	TListenerPtr pListener = nullptr;

	if (uri.schema != "https") return false;

	http_listener_config listen_config;
	listen_config.set_ssl_context_callback(ssl_context_callback);
	listen_config.set_timeout(utility::seconds(10));

	// create and open the listener (With SSL)
	pListener = std::make_shared<http_listener>(URL, listen_config);

	// update the listener pointer
	setListener(pListener);

	return startServerInternal();
}

bool RESTServer::startServerInternal()
{
	try
	{
		auto pListener = getListener();
		if (!pListener) return false;

		// general request handler
		pListener->support( [&] (http_request req) { handleRequest(req); });

		auto task = pListener->open();
		auto status = task.wait();

		if (status != concurrency::completed) return false;

		// update the server status and metadata
		setStatus(ServerInfo::ServerStatus::Listening);
		m_pCtx->ping();
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

void RESTServer::logRequest(const http_request& req) const
{
	auto uri = req.absolute_uri();
	auto method = utility::conversions::to_utf8string(req.method());
	auto path  = utility::conversions::to_utf8string(uri.path());
	auto query = utility::conversions::to_utf8string(uri.query());

	std::cout << "Request Recieved - Method: " << method << " Endpoint: " << path << " Queries: " << query << "\n";
}

void RESTServer::handleRequest(http_request req)
{
	using namespace JSONModelKeys::ResponseKeys;
	logRequest(req);
	unsigned int transactionID = 0;
	{
		// increment the transaction counter
		TLock lock(m_mutex);
		transactionID = m_transactionCounter++;

		// update the last transaction timestamp
		m_pCtx->ping();
	}
	try
	{
		// determine which endpoint we want and retrieve a pointer to it
		utility::string_t endpointName = req.relative_uri().path();
		auto pEndpoint = retrieveEndpoint(utility::conversions::to_utf8string(endpointName));
 
		if (!pEndpoint)
		{
			throw RESTServerException("Endpoint not found", ServerErrorCode::BadEndpoint);
		}

		// retrieve the method from the request and grab its ID reflection
		auto mthd = req.method();
		if (Utl::c_methodMap.find(mthd) == Utl::c_methodMap.end())
		{
			throw RESTServerException("Method reflection not found", ServerErrorCode::MethodNotSupported);
		}
	
		// call the appropriate method handler for that endpoint
		web::json::value response;
		auto m = Utl::c_methodMap.at(mthd);
		switch (m)
		{
		case Utl::RequestMethodID::GET:
			response = pEndpoint->handleGet(req, m_pCtx);
			break;
		case Utl::RequestMethodID::POST:
			response = pEndpoint->handlePost(req, m_pCtx);
			break;
		case Utl::RequestMethodID::PUT:
			response = pEndpoint->handlePut(req, m_pCtx);
			break;
		case Utl::RequestMethodID::DEL:
			response = pEndpoint->handleDelete(req, m_pCtx);
			break;
		default:
			throw RESTServerException("Method Not Recognized", ServerErrorCode::MethodNotSupported);
			break;
		}

		// append the server's transactionID
		response[c_serverXactionID] = web::json::value::number(transactionID);
		req.reply(web::http::status_codes::OK, response);
	}	
	catch (RESTServerException& err)
	{
		// this is mainly to handle bad requests
		ResponseBody resp(-1, err);
		auto errBody = resp.toJson();
		errBody[c_serverXactionID] = web::json::value::number(transactionID);
		req.reply(status_codes::BadRequest, errBody);
	}
	catch (std::exception& err)
	{
		// This is mainly to handle internal errors
		ResponseBody resp(-1, RESTServerException(err));
		auto errBody = resp.toJson();
		errBody[c_serverXactionID] = web::json::value::number(transactionID);
		req.reply(status_codes::InternalError, errBody);
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
	oss << meta.schema << "://" << meta.host << ":" << meta.port << meta.root;
	serverInfo.URLString = oss.str();
}


