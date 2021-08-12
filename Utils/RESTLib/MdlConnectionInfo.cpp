#include "stdafx.h"
#include "MdlConnectionInfo.h"


ConnectionInfoBody::ConnectionInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ConnectionInfo)
	, m_pToken(nullptr)
	, m_id(0)
{}

web::json::value ConnectionInfoBody::toJSON() const
{
	web::json::value info = web::json::value::object();

	info[U("Connection_ID")] = web::json::value::number(m_id);

	if (m_pToken)
	{
		info[m_pToken->c_name] = m_pToken->toJSON();
	}
	return info;
}

void ConnectionInfoBody::loadJSON(value info)
{
	m_id = info[U("Connection_ID")].as_integer();

	m_pToken = createBody(JSONInfoBody::BodyType::Body_TokenInfo);
	m_pToken->loadJSON(info[m_pToken->c_name]);
}