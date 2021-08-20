#include "stdafx.h"
#include "MdlErrorInfo.h"

ErrorInfoBody::ErrorInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ErrorInfo)
	, m_errorMessage("")
	, m_errorCode(ServerErrorCode::NoError)
{}

JSON ErrorInfoBody::toJSON() const
{
	auto info = JSON::object();
	auto code = static_cast<unsigned int>(m_errorCode);
	info["Error_Code"] = code;
	info["Error_Message"] = m_errorMessage;
	return info;
}

void ErrorInfoBody::loadJSON(JSON info)
{
	auto code = info["Error_Code"].get<unsigned int>();
	m_errorMessage = info["Error_Message"].get<std::string>();

	m_errorCode = static_cast<ServerErrorCode>(code);
}

void ErrorInfoBody::fromException(RESTServerException& err)
{
	m_errorCode = err.errorCode();
	m_errorMessage = err.what();
}