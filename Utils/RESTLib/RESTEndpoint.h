#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include "RESTModels.h"
#include "RESTServerContext.h"

typedef std::map<std::string, std::string> SplitQueries;

class HTTPRequestHandler
{
public:

    enum class DataType
    {
        WebPage,
        JSON,
        Unknown
    };

    HTTPRequestHandler(std::string name, DataType type)
        : m_name(name)
        , m_dataType(type)
    {}

    virtual ~HTTPRequestHandler() {};

    // virtual HTTP method handlers 
    // These will be responsible for parsing the target string for queries
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, SplitQueries queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Post(std::string target, SplitQueries queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Delete(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Head(std::string target, SplitQueries queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }

    // base property accessors
    std::string getName() const { return m_name; }
    DataType getType() const { return m_dataType; }

protected:
    std::string m_name;
    DataType    m_dataType;
};
typedef std::shared_ptr<HTTPRequestHandler>  ReqHandlerPtr;
typedef std::map<std::string, ReqHandlerPtr> ReqHandlerMap;

class WebPageRequestHandler : public HTTPRequestHandler
{

public:
    WebPageRequestHandler() : HTTPRequestHandler("WebPage", HTTPRequestHandler::DataType::WebPage) {}

    // virtual HTTP method handlers
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, SplitQueries queries, std::string body, TRESTCtxPtr pCtx) override;
};

#endif // !RESTENDPOINT_H