#ifndef ASYNCPROCTEST_H
#define ASYNCPROCTEST_H

#include <gtest\gtest.h>
#include "AsyncAction.h"
#include "TestActions.h"
class AsyncProccessorTest : public ::testing::Test
{
protected:
	void SetUp();
	void TearDown();

	std::shared_ptr<TestActionEmpty>		m_pEmptyAction;
	std::shared_ptr<TestActionSimpleAction> m_pSimpleAction;
	std::shared_ptr<TestActionTimeout>      m_pLongAction;
	std::shared_ptr<TestActionError>        m_pErrorAction;
};


#endif // !ASYNCPROCTEST_H