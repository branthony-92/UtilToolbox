#include "stdafx.h"
#include "ResultInfoBody.h"

ResultInfoBody::ResultInfoBody()
	: JSONInfoBody("Test_Result_Info")
	, m_name()
	, m_val(0)
{}

JSON ResultInfoBody::toJSON() const 
{
	JSON info = JSON::object();

	info["Name"] = m_name;
	info["JSON"] = m_val;
	return info;
}

void ResultInfoBody::loadJSON(JSON info) 
{
	m_val = info["Name"].get<int>();
	m_name = info["JSON"].get<std::string>();
}