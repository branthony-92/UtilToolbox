#include "stdafx.h"
#include "MdlErrorInfo.h"

ErrorInfoBody::ErrorInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ErrorInfo)
	, m_errorMessage("")
	, m_errorCode(ServerErrorCode::NoError)
{}

value ErrorInfoBody::toJSON() const
{
	auto info = value::object();
	auto code = static_cast<unsigned int>(m_errorCode);
	info[U("Error_Code")] = value::number(code);
	info[U("Error_Message")] = value::string(utility::conversions::to_string_t(m_errorMessage));
	return info;
}

void ErrorInfoBody::loadJSON(value info)
{
	auto code = info[U("Error_Code")].as_integer();
	auto msg = info[U("Error_Message")].as_string();

	m_errorCode = static_cast<ServerErrorCode>(code);
	m_errorMessage = utility::conversions::to_utf8string(msg);
}

void ErrorInfoBody::fromException(RESTServerException& err)
{
	m_errorCode = err.errorCode();
	m_errorMessage = err.what();
}