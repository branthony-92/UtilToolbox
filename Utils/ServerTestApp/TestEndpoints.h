#ifndef TESTENDPOINTS_H
#define TESTENDPOINTS_H

#include "RestServer.h"

class TestEndpoint_Root : public HTTPRequestHandler
{
public:
	TestEndpoint_Root(std::string name) : HTTPRequestHandler(name) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;

};

class TestEndpoint_Auth : public HTTPRequestHandler
{
public:
	TestEndpoint_Auth(std::string name) : HTTPRequestHandler(name) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Post(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;

};


class TestEndpoint_1 : public HTTPRequestHandler
{
public:
	TestEndpoint_1(std::string name) : HTTPRequestHandler(name) {}

	// virtual HTTP method handlers

	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_2 : public HTTPRequestHandler
{
public:
	TestEndpoint_2(std::string name) : HTTPRequestHandler(name) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_Add : public HTTPRequestHandler
{
public:
	TestEndpoint_Add(std::string name) : HTTPRequestHandler(name) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};
#endif // !TESTENDPOINTS_H