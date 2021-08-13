#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

#include "RESTServerContext.h"
#include "MdlResponseInfo.h"

class RESTEndpoint
{
public:
	RESTEndpoint(std::string name) 
		: m_endpointName(name)
	{}

	virtual ~RESTEndpoint() {};

	// virtual HTTP method handlers
	virtual void handleGet(web::http::http_request& req,    ReponseBodyPtr pResonse, TRESTCtxPtr pCtx);
	virtual void handlePost(web::http::http_request& req,   ReponseBodyPtr pResonse, TRESTCtxPtr pCtx);
	virtual void handlePut(web::http::http_request& req,    ReponseBodyPtr pResonse, TRESTCtxPtr pCtx);
	virtual void handleDelete(web::http::http_request& req, ReponseBodyPtr pResonse, TRESTCtxPtr pCtx);

	// base property accessors
	std::string getName() const { return m_endpointName; }

protected:
	std::string   m_endpointName;
};
typedef std::shared_ptr<RESTEndpoint> TEndpointPtr;

#endif // !RESTENDPOINT_H