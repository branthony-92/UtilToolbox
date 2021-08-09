#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "RESTEndpointContext.h"

class RESTEndpoint
{
public:
	RESTEndpoint(std::string name) 
		: m_endpointName(name)
		, m_currTransactionID(0) {}

	virtual ~RESTEndpoint() {};

	// virtual HTTP method handlers
	virtual void handleGet(web::http::http_request req, TRESTCtxPtr pCtx);
	virtual void handlePost(web::http::http_request req, TRESTCtxPtr pCtx);
	virtual void handlePut(web::http::http_request req, TRESTCtxPtr pCtx);
	virtual void handleDelete(web::http::http_request req, TRESTCtxPtr pCtx);

	// base property accessors
	std::string getName() const { return m_endpointName; }
	unsigned long getCurrTransactionID() const { return m_currTransactionID; }

	static web::json::value createErrorJsonBody(std::string msg);

protected:
	std::string   m_endpointName;
	unsigned long m_currTransactionID;
};
typedef std::shared_ptr<RESTEndpoint> TEndpointPtr;

#endif // !RESTENDPOINT_H