#ifndef SERVERTEST_H
#define SERVERTEST_H

#include "RESTServer.h"

class ServerTest : public RESTEndpointContext
{
public:


	struct TestModel {
		std::string name;
		int         val;
		TestModel()
			: name("")
			, val(0)
		{}
	};
	ServerTest()
		: RESTEndpointContext("Test Context")
		, m_testModel_1()
		, m_testModel_2()
	{
		m_testModel_1.name = "m_testModel_1";
		m_testModel_2.name = "m_testModel_2";
	}

	TestModel m_testModel_1;
	TestModel m_testModel_2;

	int addValues() const {	return m_testModel_1.val + m_testModel_2.val; };
};
typedef std::shared_ptr<ServerTest> TServerTestCtxPtr;

class TestApp {
	TServerTestCtxPtr m_pTestContext;
	TRESTServerPtr    m_pServer;

public:
	TestApp();
	~TestApp();
	void init();
	void run();
};
#endif // !SERVERTEST_H