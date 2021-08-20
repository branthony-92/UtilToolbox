#include "stdafx.h"
#include "TestEndpoints.h"
#include "ServerTest.h"
#include "ResultInfoBody.h"

std::shared_ptr<JSONInfoBody> TestEndpoint_Root::handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto model = pTestCtx->getServerInfo();
	auto pResp = std::make_shared<ResponseInfoBody>();
	pResp->addBody(model);
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_1::handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto model = pTestCtx->m_pTestModel_1;
	auto pResp = std::make_shared<ResponseInfoBody>();
	pResp->addBody(model);
	return pResp;
}

std::shared_ptr<JSONInfoBody> TestEndpoint_1::handleRequest_Put(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	auto model = pTestCtx->m_pTestModel_1;

	//auto json = req.extract_json().get();
	//pTestCtx->m_pTestModel_1->loadJSON(json);
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_2::handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	pResp->addBody(pTestCtx->m_pTestModel_2);
	return pResp;
}

std::shared_ptr<JSONInfoBody> TestEndpoint_2::handleRequest_Put(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	// auto json = req.extract_json().get();
	// pTestCtx->m_pTestModel_2->loadJSON(json);

	auto pResp = std::make_shared<ResponseInfoBody>();
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_Add::handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	//auto result = pResp->toJSON();
	//auto val = pTestCtx->addValues();
	//
	//result[U("Val")] = web::json::value::number(val);

	auto pResp = std::make_shared<ResponseInfoBody>();
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_Auth::handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	return pResp;
	/*
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
	*/
}

std::shared_ptr<JSONInfoBody> TestEndpoint_Auth::handleRequest_Post(SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	return pResp;
	/*
	auto info = pTestCtx->handleConnectionRequest(TokenInfoBody::Lifetime::Transient, 600);
	pResp->addBody(info);
	*/
}

