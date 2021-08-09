#include "stdafx.h"
#include "ServerTest.h"
#include "TestEndpoints.h"

TestApp::TestApp()
	: m_pServer(nullptr)
	, m_pTestContext(nullptr)
{
}

TestApp::~TestApp()
{
}

void TestApp::init()
{
	m_pTestContext = std::make_shared<ServerTest>();
	m_pServer = std::make_shared<RESTServer>(m_pTestContext);

	ServerURI meta;

	meta.schema = "http";
	meta.host   = "localhost";
	meta.port   = 8080;
	meta.root   = "api";

	m_pServer->updateURI(meta);

	auto endpointNameRoot = "/";
	auto endpointName_1 = "/TestEndpoint_1";
	auto endpointName_2 = "/TestEndpoint_2";
	auto endpointName_3 = "/TestEndpoint_Add";

	auto pEndpointRoot = std::make_shared<TestEndpoint_Root>(endpointNameRoot);
	auto pEndpoint1 = std::make_shared<TestEndpoint_1>(endpointName_1);
	auto pEndpoint2 = std::make_shared<TestEndpoint_2>(endpointName_2);
	auto pEndpoint3 = std::make_shared<TestEndpoint_Add>(endpointName_3);

	m_pServer->addEndpoint(endpointNameRoot, pEndpointRoot);
	m_pServer->addEndpoint(endpointName_1, pEndpoint1);
	m_pServer->addEndpoint(endpointName_2, pEndpoint2);
	m_pServer->addEndpoint(endpointName_3, pEndpoint3);

	m_pServer->startServer();
}

void TestApp::run()
{
	auto start = std::chrono::high_resolution_clock::now();
	auto dt = 0;

	// run the server for 2 min
	const int c_timeout = 60000 * 2; 
	while (dt < c_timeout)
	{
		auto now = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

		if (m_pTestContext->stopRequested())
		{
			m_pServer->stopServer();
			break;
		}
		if (m_pTestContext->resetRequested())
		{
			start = std::chrono::high_resolution_clock::now();
			// execute whatever reset code is needed
			break;
		}
	}
}
