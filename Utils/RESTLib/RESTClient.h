#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "RESTRequest.h"
#include "ActionContext.h"
#include "HTTPUtils.h"

class RESTClient : public CActionContext
{
public:
	RESTClient(std::string name = "REST Client")
		: CActionContext(name)
		, m_requests()
	{}
	virtual ~RESTClient() {}

	virtual TRESTRespPtr GET();
	virtual TRESTRespPtr POST();
	virtual TRESTRespPtr PUT();
	virtual TRESTRespPtr DEL();

protected:
	TReqQueue m_requests;
};

#endif // !RESTCLIENT_H