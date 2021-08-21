#ifndef TESTENDPOINTS_H
#define TESTENDPOINTS_H

#include "RestServer.h"

class RESTEndpoint : public HTTPRequestHandler
{
public:
	static enum class HandlerID
	{
		FirstHandler = 0,
	
		Root = FirstHandler,
		Test1,
		Test2,
		Add,
		Auth,
		Hello,
	
		NumHandlers,
	};

	RESTEndpoint(std::string name, HTTPRequestHandler::DataType type) : HTTPRequestHandler(name, type) {}

	static ReqHandlerPtr createHandler(HandlerID id);
};

class TestEndpoint_Root : public RESTEndpoint
{
public:
	TestEndpoint_Root() : RESTEndpoint("/api/", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;

};

class TestEndpoint_Auth : public RESTEndpoint
{
public:
	TestEndpoint_Auth() : RESTEndpoint("/api/Auth", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Post(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;

};


class TestEndpoint_1 : public RESTEndpoint
{
public:
	TestEndpoint_1() : RESTEndpoint("/api/TestEndpoint_1", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers

	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_2 : public RESTEndpoint
{
public:
	TestEndpoint_2() : RESTEndpoint("/api/TestEndpoint_2", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_Add : public RESTEndpoint
{
public:
	TestEndpoint_Add() : RESTEndpoint("/api/TestEndpoint_Add", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

class TestEndpoint_Hello : public RESTEndpoint
{
public:
	TestEndpoint_Hello() : RESTEndpoint("/api/Hello", HTTPRequestHandler::DataType::JSON) {}

	// virtual HTTP method handlers
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};
#endif // !TESTENDPOINTS_H