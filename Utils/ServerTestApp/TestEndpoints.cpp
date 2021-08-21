#include "stdafx.h"
#include "TestEndpoints.h"
#include "ServerTest.h"
#include "ResultInfoBody.h"



std::shared_ptr<JSONInfoBody> TestEndpoint_Root::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
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


std::shared_ptr<JSONInfoBody> TestEndpoint_1::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
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

std::shared_ptr<JSONInfoBody> TestEndpoint_1::handleRequest_Put(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	if (body.empty()) throw RESTServerException("Request Body Is Empty", ServerErrorCode::BadBody);

	auto pResp = std::make_shared<ResponseInfoBody>();
	auto model = pTestCtx->m_pTestModel_1;

	auto json = JSON::parse(body);
	pTestCtx->m_pTestModel_1->loadJSON(json);
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_2::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
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

std::shared_ptr<JSONInfoBody> TestEndpoint_2::handleRequest_Put(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();

	auto json = JSON::parse(body);
	pTestCtx->m_pTestModel_2->loadJSON(json);
	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_Add::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	auto pResult = std::make_shared<ResultInfoBody>();
	pResult->setName("Adding Test");
	pResult->setVal(pTestCtx->addValues());
	pResp->addBody(pResult);

	return pResp;
}


std::shared_ptr<JSONInfoBody> TestEndpoint_Auth::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	
	auto idData    = queries.find("Connection_ID");
	auto tokenData = queries.find("Token");

	if (idData == queries.end())
	{
		throw RESTServerException("Access denied: No ID", ServerErrorCode::AccessDenied);
	}
	if (tokenData == queries.end())
	{
		throw RESTServerException("Access denied: No Token", ServerErrorCode::AccessDenied);
	}

	unsigned int id   = static_cast<unsigned int>(std::stoi(idData->second));
	std::string token = tokenData->second;

	if (!pTestCtx->checkConnection(id, token))
	{
		throw RESTServerException("Access denied: Incorrect token/ID", ServerErrorCode::AccessDenied);
	}

	auto info = pTestCtx->getConnectionInfo(id);
		
	// return the id and token
	pResp->addBody(info);
	return pResp;
}

std::shared_ptr<JSONInfoBody> TestEndpoint_Auth::handleRequest_Post(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	
	auto info = pTestCtx->handleConnectionRequest(TokenInfoBody::Lifetime::Transient, 600);
	pResp->addBody(info);
	
	return pResp;
}

std::shared_ptr<JSONInfoBody> TestEndpoint_Hello::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto pResp = std::make_shared<ResponseInfoBody>();
	auto pModel = pTestCtx->m_pTestModel_1;
	pModel->setName("World");
	pModel->setVal(69);
	pResp->addBody(pModel);
	return pResp;
}

ReqHandlerPtr RESTEndpoint::createHandler(RESTEndpoint::HandlerID id)
{
	ReqHandlerPtr pHandler = nullptr;
	switch (id)
	{
	case HandlerID::Root:
		pHandler = std::make_shared<TestEndpoint_Root>();
		break;
	case HandlerID::Test1:
		pHandler = std::make_shared<TestEndpoint_1>();
		break;
	case HandlerID::Test2:
		pHandler = std::make_shared<TestEndpoint_2>();
		break;
	case HandlerID::Add:
		pHandler = std::make_shared<TestEndpoint_Add>();
		break;
	case HandlerID::Auth:
		pHandler =  std::make_shared<TestEndpoint_Auth>();
		break;
	case HandlerID::Hello:
		pHandler = std::make_shared<TestEndpoint_Hello>();
		break;
	default:
		break;
	}
	return pHandler;
}
