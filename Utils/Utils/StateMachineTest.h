#ifndef STATEMACHINETEST_H
#define STATEMACHINETEST_H

#include <gtest/gtest.h>
#include "TestSM.h"
#include "TestSMState.h"
#include "TestContext.h"

class StateMachineTest : public ::testing::Test
{
protected:

	TTestSMPtr      m_pTestSM;
	TTestContextPtr m_pTestCtx;
	TStatePtr       m_pInitialState;

	void SetUp();
	void TearDown();
};

#endif // !STATEMACHINETEST_H
