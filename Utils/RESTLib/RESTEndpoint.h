#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "RESTServerContext.h"

class RESTEndpoint
{
public:
	RESTEndpoint(std::string name) 
		: m_endpointName(name)
		, m_currTransactionID(0) {}

	virtual ~RESTEndpoint() {};

	// virtual HTTP method handlers
	virtual web::json::value handleGet(web::http::http_request& req, TRESTCtxPtr pCtx);
	virtual web::json::value handlePost(web::http::http_request& req, TRESTCtxPtr pCtx);
	virtual web::json::value handlePut(web::http::http_request& req, TRESTCtxPtr pCtx);
	virtual web::json::value handleDelete(web::http::http_request& req, TRESTCtxPtr pCtx);

	// base property accessors
	std::string getName() const { return m_endpointName; }
	unsigned long getCurrTransactionID() const { return m_currTransactionID; }

protected:
	std::string   m_endpointName;
	unsigned long m_currTransactionID;
};
typedef std::shared_ptr<RESTEndpoint> TEndpointPtr;

#endif // !RESTENDPOINT_H