#include "StdAfx.h"
#include "RESTEndpoint.h"
#include "MdlResponseInfo.h"

std::shared_ptr<JSONInfoBody> WebPageRequestHandler::handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx)
{
	if (!pCtx) return nullptr;
	ReponseBodyPtr pResp = std::make_shared<ResponseInfoBody>();

	auto pWebPageBody = std::make_shared<WebPageInfoBody>();
	pWebPageBody->setPath(pCtx->findResource(target));
	return pWebPageBody;
}


