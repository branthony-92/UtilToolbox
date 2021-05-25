#include "stdafx.h"
#include "AsyncProccessorTest.h"
#include "TestResults.h"
void AsyncProccessorTest::SetUp()
{
	auto pEmptyResult      = std::make_shared<TestResultEmpty>();
	auto pSimpleResult     = std::make_shared<TestResultSimple>();
	auto pComputeFibResult = std::make_shared<TestResultComputeFib>();

	m_pEmptyAction      = std::make_shared<TestActionEmpty>(pEmptyResult);
	m_pSimpleAction     = std::make_shared<TestActionSimpleAction>(pSimpleResult);
	m_pLongAction       = std::make_shared<TestActionTimeout>(pEmptyResult);
	m_pErrorAction      = std::make_shared<TestActionError>(pEmptyResult);
	m_pComputeFibAction = std::make_shared<TestActionComputeFib>(pComputeFibResult);
}

void AsyncProccessorTest::TearDown()
{
}

TEST_F(AsyncProccessorTest, SimpleEmptyActionTestThreaded)
{
	auto pResult = m_pEmptyAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pEmptyAction->run();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Complete);
}

TEST_F(AsyncProccessorTest, SimpleActionTestThreaded)
{
	auto pResult = m_pSimpleAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pSimpleAction->run();
	status = pPromise->waitForResult();
	EXPECT_EQ(status, CAsyncResult::Status::Complete);

 	auto pSimpleResult = CAsyncResult::convertTo<TestResultSimple>(pResult);
	EXPECT_TRUE(pSimpleResult != nullptr);

	EXPECT_EQ(pSimpleResult->getIntData(),    1);
	EXPECT_EQ(pSimpleResult->getFloatData(),  1.0f);
	EXPECT_EQ(pSimpleResult->getBoolData(),   true);
	EXPECT_EQ(pSimpleResult->getStringData(), std::string("Result"));
}

TEST_F(AsyncProccessorTest, ErrorEmptyActionTestThreaded)
{
	auto pResult = m_pErrorAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pErrorAction->run();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Error);
	EXPECT_FALSE(pResult->getError().empty());
}

TEST_F(AsyncProccessorTest, TimeoutEmptyActionTestThreaded)
{
	auto pResult = m_pLongAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pLongAction->run();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Timeout);
}

TEST_F(AsyncProccessorTest, ResultStatusFibTimeoutTest)
{
	auto pResult = m_pComputeFibAction->getResult();
	auto status = pResult->getStatus();
	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	m_pComputeFibAction->m_params.nFib = 50; // compute the 50th num of the fib sequence

	auto pPromise = m_pComputeFibAction->run();

	status = pPromise->waitForResult();
	
	EXPECT_EQ(status, CAsyncResult::Status::Timeout);
}

TEST_F(AsyncProccessorTest, ResultStatusMonitoringTest)
{
	auto pResult = m_pComputeFibAction->getResult();
	auto status = pResult->getStatus();
	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	m_pComputeFibAction->m_params.nFib = 15; // compute the 15th num of the fib sequence

	m_pComputeFibAction->run();

	status = pResult->getStatus();
	while (status == CAsyncResult::Status::Executing)
	{
		// monitor the status
		status = pResult->getStatus();
	}
	EXPECT_EQ(status, CAsyncResult::Status::Complete);
}