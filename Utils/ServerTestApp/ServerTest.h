#ifndef SERVERTEST_H
#define SERVERTEST_H

#include "RESTServer.h"
#include "RESTModels.h"

using namespace web;
using namespace web::json;
using namespace utility;
using namespace utility::conversions;


class ResultInfoBody : public JSONInfoBody
{
public:
	ResultInfoBody()
		: JSONInfoBody(U("Test_Result_Info"))
		, m_name()
		, m_val(0)
	{}	 

	value toJSON() const override
	{
		value info = value::object();

		info[U("Name")] = value::string(to_string_t(m_name));
		info[U("Value")] = value::number(m_val);
		return info;
	}

	void loadJSON(web::json::value info) override
	{
		m_val = info[U("Name")].as_integer();
		m_name = to_utf8string(info[U("Value")].as_string());
	}

	std::string getName() const { return m_name; }
	int getVal() const { return m_val; }

	void setName(std::string name) { m_name = name; }
	void setVal(int val) { m_val = val; }

private:
	std::string m_name;
	int         m_val;
};
typedef std::shared_ptr<ResultInfoBody> ResultInfoPtr;

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

	int addValues() const {	
		return m_pTestModel_1->getVal() + m_pTestModel_2->getVal(); 
	};

private:
};
typedef std::shared_ptr<ServerTest> TServerTestCtxPtr;

class TestApp {
	TServerTestCtxPtr m_pTestContext;
	TRESTServerPtr    m_pServer;


public:
	TestApp();
	~TestApp();
	void init(std::shared_ptr<URIInfoBody> pUri);
	void run();

	std::string URL();
};
#endif // !SERVERTEST_H