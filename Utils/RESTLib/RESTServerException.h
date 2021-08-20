#ifndef RESTSERVEREXCEPTION_H
#define RESTSERVEREXCEPTION_H

enum class ServerErrorCode
{
	NoError = 0,
	BadContext,
	BadEndpoint,
	BadMethod,
	BadBody,
	BadQuery,
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

	ServerErrorCode errorCode() { return m_error; }

private:
	ServerErrorCode m_error;
};

#endif // !RESTSERVEREXCEPTION_H