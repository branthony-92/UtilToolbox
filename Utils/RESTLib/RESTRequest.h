#ifndef RESTREQUEST_H
#define RESTREQUEST_H

#include <cpprest\uri.h>
#include <cpprest\http_client.h>
#include <map>
#include "AsyncAction.h"
#include "AsyncResult.h"
#include "HTTPUtils.h"

class RequestData {
public:
	typedef std::map<utility::string_t, utility::string_t> TQueryMap;
	RequestData()
		: m_method()
		, m_rootPath()
		, m_endpointPath()
		, m_body()
		, m_queries()
	{}

	utility::string_t  getRootPath()      const { return m_rootPath;     }
	utility::string_t  getEndpointPath()  const { return m_endpointPath; }
	web::json::value   getBody()          const { return m_body;	     }
	TQueryMap		   getQueries()       const { return m_queries;      }
	Utl::RequestMethodID getMethod()      const { return m_method;	     }

	void setRootPath(utility::string_t path)     { m_rootPath = path;     }
	void setEndpointPath(utility::string_t path) { m_endpointPath = path; }
	void setBody(web::json::value body )		 { m_body = body;		  }

	void addQuery(utility::string_t key, utility::string_t value) {
		m_queries.insert_or_assign(key, value);
	}

private:
	utility::string_t m_rootPath;
	utility::string_t m_endpointPath;
	web::json::value  m_body;
	TQueryMap		  m_queries;

	const Utl::RequestMethodID m_method;
};

class RESTResponse : public CAsyncResult
{
	web::http::http_response m_response;
public:
	RESTResponse(std::string name) 
		: CAsyncResult(name) 
		, m_response() 
	{}

	web::http::http_response getResponse() const     { return m_response; }
	void setResponse(web::http::http_response resp)  { m_response = resp; }
};
typedef std::shared_ptr<RESTResponse> TRESTRespPtr;


class RESTRequest : public CAsyncAction
{
	RequestData m_reqData;
public:

	RESTRequest(RequestData data, TRESTRespPtr pResponse, unsigned int timeout)
		: CAsyncAction(Utl::c_methodIDReflections.at(data.getMethod()), pResponse, timeout)
		, m_reqData()
	{}

	RequestData getData() const { return m_reqData;  }
protected:
	void execute(CActionContext* pCtx) override;
};
typedef std::shared_ptr<RESTRequest> TRESTReqPtr;
typedef std::list<TRESTReqPtr> TReqQueue;
#endif // !RESTREQUEST_H
