#ifndef ASYNCPROCTEST_H
#define ASYNCPROCTEST_H

#include <gtest\gtest.h>
#include "AsyncAction.h"
#include "TestActions.h"
#include "ActionContext.h"
class AsyncProccessorTest : public ::testing::Test, public CActionContext
{
public:
	AsyncProccessorTest()
		: CActionContext("TestContext")
	{}
protected:
	void SetUp();
	void TearDown();

	std::shared_ptr<TestActionEmpty>		m_pEmptyAction;
	std::shared_ptr<TestActionSimpleAction> m_pSimpleAction;
	std::shared_ptr<TestActionTimeout>      m_pLongAction;
	std::shared_ptr<TestActionError>        m_pErrorAction;
	std::shared_ptr<TestActionComputeFib>   m_pComputeFibAction;
};


#endif // !ASYNCPROCTEST_H