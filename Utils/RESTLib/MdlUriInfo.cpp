#include "stdafx.h"
#include "MdlUriInfo.h"

URIInfoBody::URIInfoBody()
	: JSONInfoBody(JSONInfoBody::BodyType::Body_URIInfo)
	, m_schema()
	, m_host()
	, m_port()
	, m_root()
{}

value URIInfoBody::toJSON() const 
{
	auto info = value::object();
	info[U("Schema")] = value::string(to_string_t(m_schema));
	info[U("Hostname")] = value::string(to_string_t(m_host));
	info[U("Port")] = value::number(m_port);
	info[U("Root_Endpoint")] = value::string(to_string_t(m_root));
	return info;
}
void URIInfoBody::loadJSON(value info) 
{
	auto schema = info[U("Schema")].as_string();
	auto host = info[U("Hostname")].as_string();
	auto port = info[U("Port")].as_integer();
	auto root = info[U("Root_Endpoint")].as_string();

	m_schema = to_utf8string(schema);
	m_host = to_utf8string(host);
	m_port = port;
	m_root = to_utf8string(root);
}