#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include "RESTModels.h"
#include "RESTServerContext.h"

typedef std::map<std::string, std::string> SplitQueries;

class HTTPRequestHandler
{
public:


    HTTPRequestHandler(std::string name)
        : m_name(name)
    {}

    virtual ~HTTPRequestHandler() {};

    // virtual HTTP method handlers 
    // These will be responsible for parsing the target string for queries
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(SplitQueries queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(SplitQueries queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Post(SplitQueries queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Delete(SplitQueries queries, std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Head(SplitQueries queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }

    // base property accessors
    std::string getName() const { return m_name; }

private:
    std::string m_name;
};
typedef std::shared_ptr<HTTPRequestHandler>  ReqHandlerPtr;
typedef std::map<std::string, ReqHandlerPtr> ReqHandlerMap;


#endif // !RESTENDPOINT_H