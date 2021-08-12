#include "stdafx.h"
#include "MdlTokenInfo.h"

TokenInfoBody::TokenInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_TokenInfo)
	, m_val("")
	, m_expirationTime()
	, m_timeout(0)
	, m_type(Lifetime::Invalid)
{}

value TokenInfoBody::toJSON() const 
{
	auto info = value::object();

	// send the timeout as a string cause it's so big
	auto expirationTimepoint = std::chrono::duration_cast<std::chrono::milliseconds>(m_expirationTime.time_since_epoch()).count();
	std::ostringstream oss;
	oss << expirationTimepoint;

	info[U("Token_Value")] = value::string(to_string_t(m_val));
	info[U("Expiration_Timepoint")] = value::string(to_string_t(oss.str()));
	info[U("Lifetime")] = value::number(m_timeout);
	info[U("Lifetime_Type")] = value::number(static_cast<unsigned int>(m_type));

	return info;
}

void TokenInfoBody::loadJSON(value info) 
{
	auto val = info[U("Token_Value")].as_string();
	auto exp = info[U("Expiration_Timepoint")].as_string();
	auto timeout = info[U("Lifetime")].as_integer();
	auto type = info[U("Lifetime_Type")].as_integer();


	auto durSinceEpoc = std::stoll(to_utf8string(exp));
	std::chrono::milliseconds t(durSinceEpoc);

	m_val = to_utf8string(val);
	m_expirationTime = TimeStamp(t);
	m_timeout = timeout;
	m_type = static_cast<Lifetime>(type);
}
