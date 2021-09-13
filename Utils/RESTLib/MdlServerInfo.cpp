#include "stdafx.h"
#include "MdlServerInfo.h"


ServerInfoBody::ServerInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_ServerInfo)
	, m_serverName("")
	, m_APIVersion(1.0)
	, m_URLString("")
	, m_pURI(std::make_shared<URIInfoBody>())
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

JSON ServerInfoBody::toJSON() const
{
	JSON info = JSON::object();

	JSON uriInfo = JSON::object();


	info["Server_Name"]     = m_serverName;
	info["API_Version"]     = m_APIVersion;
	info["API_Root_URL"]    = m_URLString;
	info["Idle_Timout_Sec"] = m_serverIdleTimoutSec;

	JSON ar = JSON::array();
	for (auto& e : m_endpointNames)
	{
		ar.push_back(e);
	}
	info["API_Endpoints"] = ar;

	if (m_pURI)
	{
		info[m_pURI->c_name] = m_pURI->toJSON();
	}
	return info;
}

void ServerInfoBody::loadJSON(JSON info)
{
	m_serverName          = jsonUtils::extractValue<std::string>(info, "Server_Name", "");
	m_APIVersion          = jsonUtils::extractValue<double>(info, "API_Version", 0.0);
	m_serverIdleTimoutSec = jsonUtils::extractValue<unsigned int>(info, "Idle_Timout_Sec", UINT32_MAX);
	m_URLString			  = jsonUtils::extractValue<std::string>(info, "API_Root_URL", "");

	auto endpoints = jsonUtils::extractValue<std::vector<std::string>>(info, "API_Endpoints", std::vector<std::string>());

	m_endpointNames.clear();
	for (auto& e : endpoints)
	{
		m_endpointNames.insert(e);
	}
	m_pURI = std::make_shared<URIInfoBody>();

	auto uriBody = jsonUtils::extractValue<JSON>(info, m_pURI->c_name, JSON::object());
	m_pURI->loadJSON(uriBody);
}