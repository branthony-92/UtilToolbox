#include "StdAfx.h"
#include "RESTEndpoint.h"

using namespace web;
using namespace web::http;
using namespace web::json;


namespace {
	const utility::string_t c_errorMsgKey = U("Error_Message");
}

void RESTEndpoint::handleGet(web::http::http_request req, TRESTCtxPtr pCtx)
{
	auto errBody = createErrorJsonBody("Endpoint does not support method GET");
	req.reply(status_codes::BadRequest, errBody);
}

void RESTEndpoint::handlePost(web::http::http_request req, TRESTCtxPtr pCtx)
{
	auto errBody = createErrorJsonBody("Endpoint does not support method POST");
	req.reply(status_codes::BadRequest, errBody);
}

void RESTEndpoint::handlePut(web::http::http_request req, TRESTCtxPtr pCtx)
{
	auto errBody = createErrorJsonBody("Endpoint does not support method PUT");
	req.reply(status_codes::BadRequest, errBody);
}

void RESTEndpoint::handleDelete(web::http::http_request req, TRESTCtxPtr pCtx)
{
	auto errBody = createErrorJsonBody("Endpoint does not support method DELETE");
	req.reply(status_codes::BadRequest, errBody);
}

json::value RESTEndpoint::createErrorJsonBody(std::string msg)
{
	auto errBody = json::value::object();
	errBody[c_errorMsgKey] = value::string(utility::conversions::to_string_t(msg));
	return errBody;
}
