// ServerTestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>

#include "ServerTest.h"
#include "TestEndpoints.h"
#include "RESTServer.h"

struct ParseArgs {

	std::map<std::string, std::string> m_args;
	std::string						   m_appName;

	void parseVal(std::string arg)
	{
		auto offset = arg.find_first_of('=');

		if (offset == std::string::npos)
		{
			m_args.insert_or_assign(arg, "");
			return;
		}
		auto key = arg.substr(0, offset);
		auto val = arg.substr(offset + 1);

		m_args.insert_or_assign(key, val);
	}

	ParseArgs(int argc, char* argv[])
		: m_args()
		, m_appName()
	{
		m_appName = argv[0];
		for (auto i = 1u; i < argc; i++)
		{
			parseVal(argv[i]);
		}
	}

	bool findFlag(const std::string& flag) const
	{
		auto flagIter = m_args.find(flag);
		return flagIter != m_args.end();
	}

	std::string getFlagValue(const std::string& flag) const {
		if (!findFlag(flag)) return "";

		return m_args.at(flag);
	}

};

int main(int argc, char* argv[])
{
	TestApp test;

	ParseArgs args(argc, argv);
	auto host    = args.getFlagValue("-host");
	auto portStr = args.getFlagValue("-port");

	auto pURI = std::make_shared<URIInfoBody>();

	std::string hostVal   = host.empty() ? "192.168.2.16" : host;
	unsigned short portVal   = portStr.empty() ? 8080 : std::stoi(portStr);


	test.init(hostVal, portVal);
	while (true);
	std::cout << "Server timed out";
}
