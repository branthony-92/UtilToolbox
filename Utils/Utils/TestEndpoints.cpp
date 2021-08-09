#include "stdafx.h"
#include "TestEndpoints.h"
#include "ServerTest.h"

void TestEndpoint_1::handleGet(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}

		auto model = pTestCtx->m_testModel_1;
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto result = web::json::value::object();
		auto name = utility::conversions::to_string_t(model.name);

		result[U("Name")] = web::json::value::string(name);
		result[U("Val")] = web::json::value::number(model.val);
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);

		req.reply(web::http::status_codes::OK, result);

	}
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}

void TestEndpoint_1::handlePut(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}

		auto& model = pTestCtx->m_testModel_1;
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto json = req.extract_json().get();

		auto name = json[U("Name")].as_string();
		auto val  = json[U("Val")].as_integer();

		model.name = utility::conversions::to_utf8string(name);
		model.val = val;

		auto result = web::json::value::object();
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);
		req.reply(web::http::status_codes::OK, result);
	}
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}


void TestEndpoint_2::handleGet(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}

		auto model = pTestCtx->m_testModel_2;
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto result = web::json::value::object();
		auto name = utility::conversions::to_string_t(model.name);

		result[U("Name")] = web::json::value::string(name);
		result[U("Val")] = web::json::value::number(model.val);
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);

		req.reply(web::http::status_codes::OK, result);

	}
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}

void TestEndpoint_2::handlePut(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}

		auto& model = pTestCtx->m_testModel_2;
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto json = req.extract_json().get();

		auto name = json[U("Name")].as_string();
		auto val = json[U("Val")].as_integer();

		model.name = utility::conversions::to_utf8string(name);
		model.val = val;

		auto result = web::json::value::object();
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);
		req.reply(web::http::status_codes::OK, result);
	}
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}


void TestEndpoint_Add::handleGet(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto result = web::json::value::object();
		auto val = pTestCtx->addValues();

		result[U("Val")] = web::json::value::number(val);
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);

		req.reply(web::http::status_codes::OK, result);
	} 
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}

void TestEndpoint_Root::handleGet(web::http::http_request req, TRESTCtxPtr pCtx)
{
	try
	{
		auto pTestCtx = std::dynamic_pointer_cast<ServerTest>(pCtx);
		if (!pTestCtx)
		{
			throw std::runtime_error("Endpoint context is invalid");
		}

		auto info = pTestCtx->serverInfo();
		auto transactionID = static_cast<unsigned int>(m_currTransactionID++);

		auto result = web::json::value::object();
		
		result[JSONModelKeys::c_serverInfoKey] = info.toJSON();
		result[JSONModelKeys::General::c_xactionID] = web::json::value::number(transactionID);

		req.reply(web::http::status_codes::OK, result);
	}
	catch (std::exception& err)
	{
		auto errBody = RESTEndpoint::createErrorJsonBody(err.what());
		req.reply(web::http::status_codes::InternalError, errBody);
	}
}
