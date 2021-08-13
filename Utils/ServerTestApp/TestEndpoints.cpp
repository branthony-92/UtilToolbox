#include "stdafx.h"
#include "TestEndpoints.h"
#include "ServerTest.h"

void TestEndpoint_1::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto model = pTestCtx->m_pTestModel_1;

	auto result = pResp->toJSON();
	auto name = utility::conversions::to_string_t(model->getName());

	result[U("Name")] = web::json::value::string(name);
	result[U("Val")]  = web::json::value::number(model->getVal());
}

void TestEndpoint_1::handlePut(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto& model = pTestCtx->m_pTestModel_1;
	auto json = req.extract_json().get();

	pTestCtx->m_pTestModel_1->loadJSON(json);
}


void TestEndpoint_2::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	pResp->addBody(pTestCtx->m_pTestModel_2);
}

void TestEndpoint_2::handlePut(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}


	auto json = req.extract_json().get();
	pTestCtx->m_pTestModel_2->loadJSON(json);
}


void TestEndpoint_Add::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto result = pResp->toJSON();
	auto val = pTestCtx->addValues();

	result[U("Val")] = web::json::value::number(val);
}

void TestEndpoint_Root::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{

	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto info = pTestCtx->serverInfo();

	pResp->addBody(info);
	auto result = pResp->toJSON();
}

void TestEndpoint_Auth::handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto queryString = req.request_uri().query();

	auto queries = req.request_uri().split_query(queryString);

	auto idData    = queries.find(U("Connection_ID"));
	auto tokenData = queries.find(U("Token"));

	if (idData == queries.end())
	{
		throw RESTServerException("Access denied: No ID", ServerErrorCode::AccessDenied);
	}
	if (tokenData == queries.end())
	{
		throw RESTServerException("Access denied: No Token", ServerErrorCode::AccessDenied);
	}

	unsigned int id = static_cast<unsigned int>(std::stoi(idData->second));
	std::string token = utility::conversions::to_utf8string(tokenData->second);

	if (!pTestCtx->checkConnection(id, token))
	{
		throw RESTServerException("Access denied: Incorrect token/ID", ServerErrorCode::AccessDenied);
	}

	auto info = pTestCtx->getConnectionInfo(id);
		
	// return the id and token
	pResp->addBody(info);
}

void TestEndpoint_Auth::handlePost(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto info = pTestCtx->handleConnectionRequest(TokenInfoBody::Lifetime::Transient, 600);
	pResp->addBody(info);
}
