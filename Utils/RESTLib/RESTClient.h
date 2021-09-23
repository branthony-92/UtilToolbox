#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "nlohmann/json.hpp"
#include "RESTModels.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using JSON = nlohmann::json;
typedef std::map<std::string, std::string> ParsedQueries;

enum class RequestStatus
{
	RequestPending,
	RequestSent,
	ResponseReady,
	Error,
};

class Request
{
public:
	Request(std::string address, std::string port, std::string target)
		: m_address(address)
		, m_port(port)
		, m_target(target)
		, m_queries()
		, m_status(RequestStatus::RequestPending)
	{}
	Request()
		: m_address("localhost")
		, m_port("0")
		, m_target("")
		, m_queries()
		, m_status(RequestStatus::RequestPending)
	{}

	std::string     m_address;
	std::string     m_port;
	std::string     m_target;
	ParsedQueries    m_queries;
	RequestStatus   m_status;
protected:
};
typedef std::shared_ptr<Request> RequestPtr;

class Response
{
	http::response<http::dynamic_body> m_resp;
	std::string m_errorMsg;

public:
	Response()
		: m_resp()
		, m_errorMsg()
	{}

	http::response<http::dynamic_body> getResponseBody() const { return m_resp; }
	std::string getErrorMsg() const { return m_errorMsg; }

	void setResponse(http::response<http::dynamic_body> response) { m_resp = response; }
	void setError(std::string msg) { m_errorMsg = msg; }

	bool hasError() const { return !m_errorMsg.empty(); }
};
typedef std::shared_ptr<Response> ResponsePtr;

class RequestPromise
{
	std::shared_future<ResponsePtr> m_future;
	RequestStatus&                  m_status;
public:
	RequestPromise(RequestStatus& status, std::shared_future<ResponsePtr> future) 
		: m_status(status)
		, m_future(future) 
	{}

	RequestStatus getStatus() const { return m_status; }
	void waitFor(std::chrono::milliseconds timeout) { m_future.wait_for(timeout); }
	ResponsePtr getResult() { return m_future.get(); }
};
typedef std::shared_ptr<RequestPromise> RequestPromisePtr;

class Client 
{
public:
	Client(std::function<void(ssl::context&)> sslContextHandler)
	: m_fSSLInitHandler(sslContextHandler)
	{}

	virtual ~Client() {}
	RequestPromisePtr makeRequest(RequestPtr pReq, http::verb verb);

protected:
	std::function<void(ssl::context&)> m_fSSLInitHandler;
};
typedef std::shared_ptr<Client> ClientPtr;

#endif // !RESTCLIENT_H