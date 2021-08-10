#ifndef RESTREQUEST_H
#define RESTREQUEST_H

#include <cpprest\uri.h>
#include <map>
#include "AsyncAction.h"
#include "HTTPUtils.h"

class RESTResponse : public CAsyncResult
{
public:
	RESTResponse();

};
typedef std::shared_ptr<RESTResponse> TRESTResponsePtr;


class RESTRequestBase : public CAsyncAction
{
public:
	typedef std::map<utility::string_t, utility::string_t> TQueryMap;

	RESTRequestBase(Utl::RequestMethodID method, TRESTResponsePtr pResponse, unsigned int timeout)
		: CAsyncAction(Utl::c_methodIDReflections.at(method), pResponse, timeout)
		, m_method(method)
		, m_path()
		, m_body()
		, m_queries()
	{}

	utility::string_t  getPath()    const { return m_path;    }
	web::json::value   getBody()    const { return m_body;	  }
	TQueryMap		   getQueries() const { return m_queries; }
	Utl::RequestMethodID getMethod() const { return m_method; }

	void setPath(utility::string_t path) { m_path = path; }
	void setBody(web::json::value body ) { m_body = body; }

	void addQuery(utility::string_t key, utility::string_t value) {
		m_queries.insert_or_assign(key, value);
	}

protected:
						       
	utility::string_t          m_path;
	web::json::value           m_body;
	TQueryMap		           m_queries;

	const Utl::RequestMethodID m_method;
};

class RESTRequestGet : public RESTRequestBase
{
public:
	RESTRequestGet(TRESTResponsePtr pResponse, unsigned int timeout)
		: RESTRequestBase(Utl::RequestMethodID::GET, pResponse, timeout)
	{}

	virtual void execute(CActionContext* pCtx) override;
};

class RESTRequestPut : public RESTRequestBase
{
public:
	RESTRequestPut(TRESTResponsePtr pResponse, unsigned int timeout)
		: RESTRequestBase(Utl::RequestMethodID::PUT, pResponse, timeout)
	{}
	virtual void execute(CActionContext* pCtx) override;
};

class RESTRequestPost : public RESTRequestBase
{
	RESTRequestPost(TRESTResponsePtr pResponse, unsigned int timeout)
		: RESTRequestBase(Utl::RequestMethodID::POST, pResponse, timeout)
	{}
	virtual void execute(CActionContext* pCtx) override;
};

class RESTRequestDelete : public RESTRequestBase
{
	RESTRequestDelete(TRESTResponsePtr pResponse, unsigned int timeout)
		: RESTRequestBase(Utl::RequestMethodID::DEL, pResponse, timeout)
	{}
	virtual void execute(CActionContext* pCtx) override;
};
#endif // !RESTREQUEST_H
