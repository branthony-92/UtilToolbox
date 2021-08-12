#include "stdafx.h"
#include "MdlServerInfo.h"


ServerInfoBody::ServerInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ServerInfo)
	, m_serverName("")
	, m_APIVersion(1.0)
	, m_URLString("")
	, m_pURI(nullptr)
	, m_serverIdleTimoutSec(UINT_MAX) // basically forever
	, m_serverState(ServerStatus::Uninitialized)
	, m_endpointNames()
{}

std::string ServerInfoBody::stringFromState(ServerStatus state)
{
	std::string name = "";

	switch (state)
	{
	case ServerStatus::Uninitialized: name = "Uninitialized"; break;
	case ServerStatus::Listening:     name = "Listening"; break;
	case ServerStatus::Error:         name = "Listening"; break;
	default:                          name = "Unknown "; break;
	}
	return name;
}

web::json::value ServerInfoBody::toJSON() const
{
	web::json::value info = web::json::value::object();

	web::json::value uriInfo = web::json::value::object();

	auto name = utility::conversions::to_string_t(m_serverName);
	auto url = utility::conversions::to_string_t(m_URLString);

	info[U("Server_Name")] = web::json::value::string(name);
	info[U("API_Version")] = web::json::value::number(m_APIVersion);
	info[U("API_Root_URL")] = web::json::value::string(url);
	info[U("Idle_Timout_Sec")] = web::json::value::number(m_serverIdleTimoutSec);

	web::json::value ar = web::json::value::array(m_endpointNames.size());

	auto index = 0u;
	for (auto& e : m_endpointNames)
	{
		auto ep = utility::conversions::to_string_t(e);
		ar[index++] = web::json::value::string(ep);
	}
	info[U("API_Endpoints")] = ar;

	if (m_pURI)
	{
		info[m_pURI->c_name] = m_pURI->toJSON();
	}
	return info;
}

void ServerInfoBody::loadJSON(value info)
{
	auto name = info[U("Server_Name")].as_string();
	auto apiVer = info[U("API_Version")].as_number();
	auto url = info[U("API_Root_URL")].as_string();
	auto timeout = info[U("Idle_Timout_Sec")].as_number();

	auto endpoints = info[U("API_Endpoints")].as_array();

	m_endpointNames.clear();
	for (auto& e : endpoints)
	{
		auto name = to_utf8string(e.as_string());
		m_endpointNames.insert(name);
	}
	m_pURI = createBody(JSONInfoBody::BodyType::Body_URIInfo);
	m_pURI->loadJSON(info[m_pURI->c_name]);
}