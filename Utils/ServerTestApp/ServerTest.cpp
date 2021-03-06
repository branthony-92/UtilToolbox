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

void TestApp::init(std::string address, unsigned short port)
{
	// create the context object and inject it into the server object via it's contructor

	// the majority of the business logic should occur in the context class and directed by the endpoint classes virtual handle methods
	m_pTestContext = std::make_shared<ServerTest>();
	m_pServer      = std::make_shared<Server>(m_pTestContext, 8);


	// define the URI and pass it to the server

	// store the server's metadata in the context

	auto pInfo = m_pServer->getServerInfo();
	pInfo->setServerName("Brian Test Server");
	pInfo->setAPIVer(1.0);
	pInfo->setIdleTimeout(300u); // 5 min timeout
	m_pTestContext->setServerInfo(pInfo);
	
	// create our endpoint strings and and objects to be injected into the server
	for (auto i = static_cast<unsigned int>(RESTEndpoint::HandlerID::FirstHandler); i < static_cast<unsigned int>(RESTEndpoint::HandlerID::NumHandlers); i++)
	{
		auto ID = static_cast<RESTEndpoint::HandlerID>(i);
		auto pHandler = RESTEndpoint::createHandler(ID);
		if (pHandler)
		{
			m_pTestContext->registerHandler(pHandler->getName(), pHandler);
		}
	}
	// spin it up
	
	auto ssl_callback = [](ssl::context& ctx) {
		try {
			printf("SSL Context Callback entered\n");
			ctx.set_options(
				ssl::context::default_workarounds |
				ssl::context::no_sslv2 | 
				ssl::context::no_tlsv1 |
				ssl::context::no_tlsv1_1 |
				ssl::context::single_dh_use
			);

			ctx.set_password_callback(
				[](std::size_t /*max_length*/, ssl::context::password_purpose /*purpose*/) 
					{ return "test"; }
			);
			beast::error_code ec;
			ctx.use_certificate_chain_file("C:\\Windows\\SysWOW64\\newcert.pem");
			ctx.use_private_key_file("C:\\Windows\\SysWOW64\\privkey.pem", ssl::context::pem);
			ctx.use_tmp_dh_file("C:\\Users\\xs_br\\source\\repos\\openssl-1.1\\x64\\bin\\dh2048.pem");
		}
		catch (std::exception const& e) {
			std::clog << "ERROR: " << e.what() << "\n";
		}
	};

	auto pSessionPrototyp = std::make_shared<GenericSession>(m_pTestContext);
	m_pServer->startServer(address, port, pSessionPrototyp, ssl_callback);
}
