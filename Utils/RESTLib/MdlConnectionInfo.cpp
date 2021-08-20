#include "stdafx.h"
#include "MdlConnectionInfo.h"


ConnectionInfoBody::ConnectionInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ConnectionInfo)
	, m_pToken(nullptr)
	, m_id(0)
{}

JSON ConnectionInfoBody::toJSON() const
{
	JSON info = JSON::object();

	info["Connection_ID"] = m_id;

	if (m_pToken)
	{
		info[m_pToken->c_name] = m_pToken->toJSON();
	}
	return info;
}

void ConnectionInfoBody::loadJSON(JSON info)
{
	m_id = info["Connection_ID"].get<unsigned int>();

	m_pToken = std::make_shared<TokenInfoBody>();
	m_pToken->loadJSON(info[m_pToken->c_name]);
}