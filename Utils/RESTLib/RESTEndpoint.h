#ifndef RESTENDPOINT_H
#define RESTENDPOINT_H

#include "RESTModels.h"
#include "RESTServerContext.h"
#include "MdlResponseInfo.h"

typedef std::map<std::string, std::string> SplitQueries;

class QueryList
{
	SplitQueries m_queries;
public:
    QueryList(SplitQueries queries) : m_queries(queries) {}
	virtual ~QueryList() {}

	bool contains(const std::string key)
	{
		return m_queries.find(key) != m_queries.end();
	}
	std::string extract(std::string key) 
	{
		if (!contains(key)) return "";
		return m_queries.at(key);
	}
	int extract(std::string key, int def)
	{
		if (!contains(key)) return def;
		auto val = m_queries.at(key);
		try
		{
			auto result = std::stoi(val);
			return result;
		}
		catch (...)
		{
			return def;
		}
	}
	long extract(std::string key, long def)
	{
		if (!contains(key)) return def;
		auto val = m_queries.at(key);
		try
		{
			auto result = std::stol(val);
			return result;
		}
		catch (...)
		{
			return def;
		}
	}
	float  extract(std::string key, float  def)
	{
		if (!contains(key)) return def;
		auto val = m_queries.at(key);
		try
		{
			auto result = std::stof(val);
			return result;
		}
		catch (...)
		{
			return def;
		}
	}
	double extract(std::string key, double def)
	{
		if (!contains(key)) return def;
		auto val = m_queries.at(key);
		try
		{
			auto result = std::stod(val);
			return result;
		}
		catch (...)
		{
			return def;
		}
	}
	bool extract(std::string key, bool def)
	{
		if (!contains(key))      return def;
		if (key == "true")		 return true;
		else if (key == "false") return false;
		return def;
	}
};

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
        , m_pResponseBody(nullptr)
    {}

    virtual ~HTTPRequestHandler() {};

    // virtual HTTP method handlers 
    // These will be responsible for parsing the target string for queries
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Put(std::string target, QueryList queries,    std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Post(std::string target, QueryList queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Delete(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx) { return nullptr; }
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Head(std::string target, QueryList queries,   std::string body, TRESTCtxPtr pCtx) { return nullptr; }

    // base property accessors
    std::string getName() const { return m_name; }
    DataType getType() const { return m_dataType; }
    ReponseBodyPtr getResponseBody() const { return m_pResponseBody; }
    void resetResponseBody() { m_pResponseBody = std::make_shared<ResponseInfoBody>(); }

protected:
    std::string m_name;
    DataType    m_dataType;
    ReponseBodyPtr m_pResponseBody;
};
typedef std::shared_ptr<HTTPRequestHandler>  ReqHandlerPtr;
typedef std::map<std::string, ReqHandlerPtr> ReqHandlerMap;

class WebPageRequestHandler : public HTTPRequestHandler
{

public:
    WebPageRequestHandler() : HTTPRequestHandler("WebPage", HTTPRequestHandler::DataType::WebPage) {}

    // virtual HTTP method handlers
    virtual std::shared_ptr<JSONInfoBody> handleRequest_Get(std::string target, QueryList queries, std::string body, TRESTCtxPtr pCtx) override;
};

#endif // !RESTENDPOINT_H