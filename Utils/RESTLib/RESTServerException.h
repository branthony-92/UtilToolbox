#ifndef RESTSERVEREXCEPTION_H
#define RESTSERVEREXCEPTION_H

#include "JSONModelKeys.h"

using namespace web;
using namespace web::json;

enum class ServerErrorCode
{
	NoError = 0,
	BadContext,
	BadEndpoint,
	BadMethod,
	MethodNotSupported,
	AccessDenied,
	Generic,
};

class RESTServerException : public std::exception
{
public:

	RESTServerException(std::string msg, ServerErrorCode errID = ServerErrorCode::NoError)
		: std::exception(msg.c_str())
		, m_error(errID)
	{}

	RESTServerException(std::exception ex, ServerErrorCode errID = ServerErrorCode::Generic)
		: std::exception(ex)
		, m_error(errID)
	{}

	ServerErrorCode getErrorType() { return m_error; }

	json::value toJson()
	{
		using namespace JSONModelKeys::ResponseKeys;

		std::string msg = what();
		auto errCode = static_cast<unsigned int>(m_error);
		auto errBody = json::value::object();

		errBody[ErrorInfoKeys::c_errorMsgKey]  = value::string(utility::conversions::to_string_t(what()));
		errBody[ErrorInfoKeys::c_errorCodeKey] = value::number(errCode);
		return errBody;
	}

private:
	ServerErrorCode m_error;
};

#endif // !RESTSERVEREXCEPTION_H