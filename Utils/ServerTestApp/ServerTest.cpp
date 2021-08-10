#include "stdafx.h"
#include "ServerTest.h"
#include "TestEndpoints.h"

using namespace std::chrono_literals;

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
	// create the context object and inject it into the server object via it's contructor

	// the majority of the business logic should occur in the context class and directed by the endpoint classes virtual handle methods
	m_pTestContext = std::make_shared<ServerTest>();
	m_pServer      = std::make_shared<RESTServer>(m_pTestContext);

	// define the URI and pass it to the server
	ServerURI uri;
	uri.schema = "http";
	uri.host   = "localhost";
	uri.port   = 8080;
	uri.root   = "api";
	m_pServer->updateURI(uri);

	// store the server's metadata in the context
	auto& info = m_pTestContext->serverInfo();

	info.serverName = "Brian Test Server";
	info.APIVersion = 1.0;
	info.serverIdleTimoutSec = 300u; // 5 min timeout
	
	// create our endpoint strings and and objects to be injected into the server
	auto endpointNameRoot = "/";
	auto endpointNameAuth = "/Auth";
	auto endpointName_1   = "/TestEndpoint_1";
	auto endpointName_2   = "/TestEndpoint_2";
	auto endpointName_3   = "/TestEndpoint_Add";

	auto pEndpointRoot = std::make_shared<TestEndpoint_Root>(endpointNameRoot);
	auto pEndpointAuth = std::make_shared<TestEndpoint_Auth>(endpointNameAuth);
	auto pEndpoint1    = std::make_shared<TestEndpoint_1>(endpointName_1);
	auto pEndpoint2    = std::make_shared<TestEndpoint_2>(endpointName_2);
	auto pEndpoint3    = std::make_shared<TestEndpoint_Add>(endpointName_3);

	m_pServer->addEndpoint(endpointNameRoot, pEndpointRoot);
	m_pServer->addEndpoint(endpointNameAuth, pEndpointAuth);
	m_pServer->addEndpoint(endpointName_1,   pEndpoint1);
	m_pServer->addEndpoint(endpointName_2,   pEndpoint2);
	m_pServer->addEndpoint(endpointName_3,   pEndpoint3);

	// spin it up
	m_pServer->startServer();
}

void TestApp::run()
{
	while (true)
	{
		// this will check the timestamp of the last transaction 
		// and flag the server to shut down if idle for too long
		m_pTestContext->checkTimeout();

		if (m_pTestContext->stopRequested())
		{
			std::cout << "Stop flag Detected\n";
			m_pServer->stopServer();
			break;
		}
		if (m_pTestContext->resetRequested())
		{
			m_pTestContext->ping();
			break;
		}
		std::this_thread::sleep_for(100ms);
	}
}

std::string TestApp::URL()
{
	return m_pServer->getURL();
}
