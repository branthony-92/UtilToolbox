#include "stdafx.h"
#include "PasswordGeneratorEP.h"
#include "PasswordGenerator.h"
#include "MdlResponseInfo.h"

std::shared_ptr<JSONInfoBody> PasswordGeneratorEP::handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx)
{
    auto pPasswordCtx = std::dynamic_pointer_cast<PasswordGenerator>(pCtx);
    if (!pPasswordCtx)
    {
        return nullptr;
    }
    auto pInfo = pPasswordCtx->getInfo();

    m_pResponseBody->addBody(pInfo);
    return m_pResponseBody;
}

std::shared_ptr<JSONInfoBody> PasswordGeneratorEP::handleRequest_Put(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx)
{
    auto pPasswordCtx = std::dynamic_pointer_cast<PasswordGenerator>(pCtx);
    if (!pPasswordCtx)
    {
        return nullptr;
    }
    auto pInfo = pPasswordCtx->getInfo();
    JSON j = JSON::parse(body);

    pInfo->loadJSON(j);

    auto len = pInfo->getPasswordLen();

    auto charLen = static_cast<unsigned int>(0.5 * len);
    auto specialCharLen = static_cast<unsigned int>(0.1 * len);
    auto numLen = static_cast<unsigned int>(0.4 * len);

    while (charLen + specialCharLen + numLen != len) charLen++;

    pInfo->setCharLen(charLen);
    pInfo->setSpecialCharLen(specialCharLen);
    pInfo->setNumLen(numLen);

    pPasswordCtx->setInfo(pInfo);

    auto password = pPasswordCtx->generate(3);
    pInfo->setPassword(password);
    m_pResponseBody->addBody(pInfo);
    return m_pResponseBody;
}
