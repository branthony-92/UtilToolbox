#include "stdafx.h"
#include "RESTRequest.h"
#include "RESTClient.h"

void RESTRequest::execute(CActionContext* pCtx)
{
	// get the pointers we need
	auto pClient = dynamic_cast<RESTClient*>(pCtx);
	if (!pClient)  throw std::exception("Bad Client Context Pointer");
	
	auto pResult = CAsyncResult::convertTo<RESTResponse>(m_pResult);
	if (!pResult) throw std::exception("Bad response Pointer");

	auto rootPath     = m_reqData.getRootPath();
	auto endpointPath = m_reqData.getEndpointPath();
	auto queries      = m_reqData.getQueries();
	auto methodID       = m_reqData.getMethod();
	auto body         = m_reqData.getBody();

	// construct the request
	web::http::client::http_client client(rootPath);
	web::http::uri_builder builder(endpointPath);

	for (auto& q : queries)
	{
		builder.append_query(q.first, q.second);
	}

	web::http::method method;

	// determine the method
	switch (methodID)
	{
	case Utl::RequestMethodID::GET:  method = web::http::methods::GET;  break; 
	case Utl::RequestMethodID::POST: method = web::http::methods::POST; break;
	case Utl::RequestMethodID::PUT:  method = web::http::methods::PUT;  break;
	case Utl::RequestMethodID::DEL:  method = web::http::methods::DEL;  break;
	default:
		throw std::exception("Invalid method requested");
		break;
	}

	// set the request, wait for the response, then store the result
	auto resp = client.request(method, builder.to_string(), body).get();
	pResult->setResponse(resp);
}
