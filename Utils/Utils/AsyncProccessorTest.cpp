#include "stdafx.h"
#include "AsyncProccessorTest.h"
#include "TestResults.h"
void AsyncProccessorTest::SetUp()
{
	auto pEmptyResult  = std::make_shared<TestResultEmpty>();
	auto pSimpleResult = std::make_shared<TestResultSimple>();

	m_pEmptyAction  = std::make_shared<TestActionEmpty>(pEmptyResult);
	m_pSimpleAction = std::make_shared<TestActionSimpleAction>(pSimpleResult);
	m_pLongAction   = std::make_shared<TestActionTimeout>(pEmptyResult);
	m_pErrorAction  = std::make_shared<TestActionError>(pEmptyResult);
}

void AsyncProccessorTest::TearDown()
{
}

TEST_F(AsyncProccessorTest, SimpleEmptyActionTestUnthreaded)
{
	auto pResult = m_pEmptyAction->getResult();
	auto status = pResult->getStatus();
	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	status = m_pEmptyAction->runActionUnthreaded();
	                      
	EXPECT_EQ(status, CAsyncResult::Status::Complete);
}

TEST_F(AsyncProccessorTest, SimpleEmptyActionTestThreaded)
{
	auto pResult = m_pEmptyAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pEmptyAction->runActionAsync();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Complete);
}

TEST_F(AsyncProccessorTest, SimpleActionTestUnthreaded)
{
	auto pResult = m_pSimpleAction->getResult();
	auto status = pResult->getStatus();
	EXPECT_EQ(status, CAsyncResult::Status::Idle);
	m_pSimpleAction->runActionUnthreaded();
	status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Complete);

	auto pSimpleResult = CAsyncResult::convertTo<TestResultSimple>(pResult);
	EXPECT_EQ(pSimpleResult->getIntData(), 1);
	EXPECT_EQ(pSimpleResult->getFloatData(), 1.0f);
	EXPECT_EQ(pSimpleResult->getBoolData(), true);
	EXPECT_EQ(pSimpleResult->getStringData(), std::string("Result"));
}

TEST_F(AsyncProccessorTest, SimpleActionTestThreaded)
{
	auto pResult = m_pSimpleAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pSimpleAction->runActionAsync();
	status = pPromise->waitForResult();
	EXPECT_EQ(status, CAsyncResult::Status::Complete);

	auto pSimpleResult = CAsyncResult::convertTo<TestResultSimple>(pResult);
	EXPECT_EQ(pSimpleResult->getIntData(), 1);
	EXPECT_EQ(pSimpleResult->getFloatData(), 1.0f);
	EXPECT_EQ(pSimpleResult->getBoolData(), true);
	EXPECT_EQ(pSimpleResult->getStringData(), std::string("Result"));
}

TEST_F(AsyncProccessorTest, ErrorActionTestUnthreaded)
{
	auto pResult = m_pErrorAction->getResult();
	auto status = pResult->getStatus();
	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	status = m_pErrorAction->runActionUnthreaded();

	EXPECT_EQ(status, CAsyncResult::Status::Error);
	EXPECT_FALSE(pResult->getError().empty());
}

TEST_F(AsyncProccessorTest, ErrorEmptyActionTestThreaded)
{
	auto pResult = m_pErrorAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pErrorAction->runActionAsync();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Error);
	EXPECT_FALSE(pResult->getError().empty());
}

TEST_F(AsyncProccessorTest, TimeoutEmptyActionTestThreaded)
{
	auto pResult = m_pLongAction->getResult();
	auto status = pResult->getStatus();

	EXPECT_EQ(status, CAsyncResult::Status::Idle);

	auto pPromise = m_pLongAction->runActionAsync();
	status = pPromise->waitForResult();

	EXPECT_EQ(status, CAsyncResult::Status::Timeout);
	m_pLongAction->abort();
}