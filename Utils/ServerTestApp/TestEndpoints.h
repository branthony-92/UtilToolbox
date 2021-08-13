#ifndef TESTENDPOINTS_H
#define TESTENDPOINTS_H

#include "RestServer.h"

class TestEndpoint_Root : public RESTEndpoint
{
public:
	TestEndpoint_Root(std::string name) : RESTEndpoint(name) {}
	// virtual HTTP method handlers
	void handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_Auth : public RESTEndpoint
{
public:
	TestEndpoint_Auth(std::string name) : RESTEndpoint(name) {}
	// virtual HTTP method handlers
	void handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
	void handlePost(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
};


class TestEndpoint_1 : public RESTEndpoint
{
public:
	TestEndpoint_1(std::string name) : RESTEndpoint(name) {}

	// virtual HTTP method handlers
	void handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
	void handlePut(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_2 : public RESTEndpoint
{
public:
	TestEndpoint_2(std::string name) : RESTEndpoint(name) {}

	// virtual HTTP method handlers
	void handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
	void handlePut(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_Add : public RESTEndpoint
{
public:
	TestEndpoint_Add(std::string name) : RESTEndpoint(name) {}

	// virtual HTTP method handlers
	void handleGet(web::http::http_request& req, ReponseBodyPtr pResp, TRESTCtxPtr pCtx) override;
};
#endif // !TESTENDPOINTS_H