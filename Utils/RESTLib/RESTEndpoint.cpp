#include "StdAfx.h"
#include "RESTEndpoint.h"

#include "RESTServerException.h"

using namespace web;
using namespace web::http;
using namespace web::json;


web::json::value RESTEndpoint::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method GET", ServerErrorCode::MethodNotSupported);
}

web::json::value RESTEndpoint::handlePost(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method POST", ServerErrorCode::MethodNotSupported);
}

web::json::value RESTEndpoint::handlePut(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method PUT", ServerErrorCode::MethodNotSupported);
}

web::json::value RESTEndpoint::handleDelete(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method DELETE", ServerErrorCode::MethodNotSupported);
}


