#include "StdAfx.h"
#include "RESTEndpoint.h"

#include "RESTServerException.h"

using namespace web;
using namespace web::http;
using namespace web::json;


void RESTEndpoint::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method GET", ServerErrorCode::MethodNotSupported);
}

void RESTEndpoint::handlePost(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method POST", ServerErrorCode::MethodNotSupported);
}

void RESTEndpoint::handlePut(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method PUT", ServerErrorCode::MethodNotSupported);
}

void RESTEndpoint::handleDelete(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	throw RESTServerException("Endpoint does not support method DELETE", ServerErrorCode::MethodNotSupported);
}


