#ifndef STATEMACHINETEST_H
#define STATEMACHINETEST_H

#include <gtest/gtest.h>
#include "TestSM.h"

class CSMTestMgr;
class CTestSM;
class CTestContext;


class StateMachineTest : public ::testing::Test
{
protected:

	std::shared_ptr<CSMTestMgr>		   m_pSMTestMgr;
	std::shared_ptr<CTestSM>		   m_pTestSM;
	std::shared_ptr<CTestContext>      m_pTestCtx;
	TStatePtr						   m_pInitialState;

	void SetUp();
	void TearDown();
};

#endif // !STATEMACHINETEST_H
