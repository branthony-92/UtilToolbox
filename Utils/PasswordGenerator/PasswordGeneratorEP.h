#ifndef PWGEP_H
#define PWGEP_H

#include "RESTEndpoint.h"

class PasswordGeneratorEP : public HTTPRequestHandler
{
public:
	PasswordGeneratorEP() 
		: HTTPRequestHandler("/api/v1/passwordmanager/generator", HTTPRequestHandler::DataType::JSON) 
	{}
	virtual ~PasswordGeneratorEP() {}

	virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx) override;
	virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx) override;
};


#endif // !PWGEP_H