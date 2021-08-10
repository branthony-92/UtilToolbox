#include "stdafx.h"
#include "TestEndpoints.h"
#include "ServerTest.h"

web::json::value TestEndpoint_1::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto model = pTestCtx->m_testModel_1;
	auto transactionID = static_cast<unsigned long>(m_currTransactionID++);

	ResponseBody resp(transactionID);

	auto result = resp.toJson();
	auto name = utility::conversions::to_string_t(model.name);

	result[U("Name")] = web::json::value::string(name);
	result[U("Val")] = web::json::value::number(model.val);

	return result;
}

web::json::value TestEndpoint_1::handlePut(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto& model = pTestCtx->m_testModel_1;
	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);
	auto json = req.extract_json().get();

	auto name = json[U("Name")].as_string();
	auto val = json[U("Val")].as_integer();

	model.name = utility::conversions::to_utf8string(name);
	model.val = val;

	ResponseBody resp(transactionID);
	auto result = resp.toJson();
	return result;
}


web::json::value TestEndpoint_2::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto model = pTestCtx->m_testModel_2;
	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

	ResponseBody resp(transactionID);

	auto result = resp.toJson();
	auto name = utility::conversions::to_string_t(model.name);

	result[U("Name")] = web::json::value::string(name);
	result[U("Val")]  = web::json::value::number(model.val);

	return result;
}

web::json::value TestEndpoint_2::handlePut(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto& model = pTestCtx->m_testModel_2;
	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

	auto json = req.extract_json().get();

	auto name = json[U("Name")].as_string();
	auto val  = json[U("Val")].as_integer();

	model.name = utility::conversions::to_utf8string(name);
	model.val = val;

	ResponseBody resp(transactionID);
	auto result = resp.toJson();
	return result;
}


web::json::value TestEndpoint_Add::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}
	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

	ResponseBody resp(transactionID);

	auto result = resp.toJson();
	auto val = pTestCtx->addValues();

	result[U("Val")] = web::json::value::number(val);
	return result;
}

web::json::value TestEndpoint_Root::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{

	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto info = pTestCtx->serverInfo();
	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

	ResponseBody resp(transactionID);

	auto result = resp.toJson();
	result[JSONModelKeys::ServerInfoKeys::c_headerKey] = info.toJSON();

	return result;
}

web::json::value TestEndpoint_Auth::handleGet(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

	ResponseBody resp(transactionID);
	auto result = resp.toJson();

	auto queryString = req.request_uri().query();

	auto queries = req.request_uri().split_query(queryString);

	auto idData    = queries.find(JSONModelKeys::ConnectionInfoKeys::c_connectionIDKey);
	auto tokenData = queries.find(JSONModelKeys::ConnectionInfoKeys::c_connectionTokenKey);

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
	result[JSONModelKeys::ConnectionInfoKeys::c_headerKey] = info.toJSON();

	return result;
}

web::json::value TestEndpoint_Auth::handlePost(web::http::http_request& req, TRESTCtxPtr pCtx)
{
	
	auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
	if (!pTestCtx)
	{
		throw RESTServerException("Endpoint context is invalid", ServerErrorCode::BadContext);
	}

	auto transactionID = static_cast<unsigned int>(m_currTransactionID++);
	ResponseBody resp(transactionID);
	auto result = resp.toJson();

	auto info = pTestCtx->handleConnectionRequest(ConnectionManager::TokenType::Transient, 600);

	// return the id and token
	result[JSONModelKeys::ConnectionInfoKeys::c_headerKey] = info.toJSON();

	return result;
}
