#include "StdAfx.h"
#include "RESTEndpoint.h"


std::shared_ptr<JSONInfoBody> WebPageRequestHandler::handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx)
{
	if (!pCtx) return nullptr;
	auto pWebPageBody = std::make_shared<WebPageInfoBody>();
	pWebPageBody->setPath(pCtx->findResource(target));
	return pWebPageBody;
}


