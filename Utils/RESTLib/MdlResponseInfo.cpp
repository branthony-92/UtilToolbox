#include "stdafx.h"
#include "MdlResponseInfo.h"


ResponseInfoBody::ResponseInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ReponseInfo)
	, m_bodies()
	, m_transactionID()
{
}

value ResponseInfoBody::toJSON() const
{
	auto info = web::json::value::object();
	info[U("Transaction_ID")] = web::json::value::number(m_transactionID);

	for (auto b : m_bodies)
	{
		auto name = b.first;
		auto pBody = b.second;
		info[name] = pBody->toJSON();
	}
	return info;
}

void ResponseInfoBody::loadJSON(value info)
{
	m_transactionID = info[U("Transaction_ID")].as_integer();

	auto JSONObj = info.as_object();

	for (auto& j : JSONObj)
	{
		// look for the bodies we may want and add it to the response body list	
		auto pBody = JSONInfoBody::createBody(j.first);
		if (pBody)
		{
			pBody->loadJSON(j.second);
			m_bodies.insert_or_assign(pBody->c_name, pBody);
		}
	}
}

void ResponseInfoBody::addBody(std::shared_ptr<JSONInfoBody> pBody)
{
	m_bodies.insert_or_assign(pBody->c_name, pBody);
}

std::shared_ptr<JSONInfoBody> ResponseInfoBody::findBody(string_t name) const
{
	auto bodyIter = m_bodies.find(name);
	if (bodyIter != m_bodies.end())
	{
		auto pBody = bodyIter->second;
		return pBody;
	}
	return nullptr;
}
