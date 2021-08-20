#ifndef SERVERTEST_H
#define SERVERTEST_H

#include "RESTServer.h"
#include "RESTModels.h"
#include "ResultInfoBody.h"

class ServerTest : public RESTServerContext
{
public:
	ServerTest()
		: RESTServerContext("Test Context")
		, m_pTestModel_1(std::make_shared<ResultInfoBody>())
		, m_pTestModel_2(std::make_shared<ResultInfoBody>())
	{
		m_pTestModel_1->setName("m_testModel_1");
		m_pTestModel_2->setName("m_testModel_2");
	}

	ResultInfoPtr m_pTestModel_1;
	ResultInfoPtr m_pTestModel_2;

	int addJSONs() const {	
		return m_pTestModel_1->getVal() + m_pTestModel_2->getVal(); 
	};

private:
};
typedef std::shared_ptr<ServerTest> TServerTestCtxPtr;

class TestApp {
	TServerTestCtxPtr m_pTestContext;
	ServerPtr    m_pServer;


public:
	TestApp();
	~TestApp();
	void init(std::string address, unsigned short port);
};
#endif // !SERVERTEST_H