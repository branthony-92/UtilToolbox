#include "stdafx.h"
#include "StateMachineTest.h"
#include "TestSMState.h"
void StateMachineTest::SetUp()
{
	m_pTestCtx = std::make_shared<CTestContext>();
	m_pTestSM  = std::make_unique<CTestSM>(m_pTestCtx);
	m_pInitialState = std::make_shared<CSMTestStateIdle>();
	

	auto stateFirst = static_cast<unsigned int>(CSMTestState::StateID::eStateFirst);
	auto stateCount = static_cast<unsigned int>(CSMTestState::StateID::eStateCount);
	TStateList stateTable;
	// init our state list and transition tables
	for (auto i = stateFirst; i < stateCount; i++)
	{
		auto id = static_cast<CSMTestState::StateID>(i);
		auto pState = CSMTestState::createState(id);

		if (pState)
		{
			pState->initTransitionTable();
			stateTable.push_back(pState);
		}
	}
	auto idleIndex = static_cast<unsigned int>(CSMTestState::StateID::eStateIdle);
	m_pInitialState = stateTable[idleIndex];
	m_pTestSM->setStateTable(stateTable);
}

void StateMachineTest::TearDown()
{
	m_pTestCtx = nullptr;
	m_pTestSM  = nullptr;
	m_pInitialState = nullptr;
}

TEST_F(StateMachineTest, TestStateTransitions)
{
	// Create the state and init it's state table
	auto pStateOne = std::make_shared<CSMTestStateOne>();
	pStateOne->initTransitionTable();
	m_pTestSM->setCurrentState(pStateOne);

	TStatePtr pNextState = nullptr;

	// test the transitions specified in the transition table
	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventNext));
	EXPECT_TRUE(pNextState != nullptr);

	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventStop));
	EXPECT_TRUE(pNextState != nullptr);

	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventAbort));
	EXPECT_TRUE(pNextState != nullptr);

	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventError));
	EXPECT_TRUE(pNextState != nullptr);

	// Test transitions not specified in transition table
	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventRecover));
	EXPECT_TRUE(pNextState == nullptr);

	pNextState = m_pTestSM->getNextState(static_cast<uint32_t>(CTestSMEvent::EventID::eEventStart));
	EXPECT_TRUE(pNextState == nullptr);
}

TEST_F(StateMachineTest, TestStateMachineStateControl)
{
	m_pTestSM->setCurrentState(m_pInitialState);
	auto pCureState = m_pTestSM->getCurrentState();

	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateIdle);
	m_pTestCtx->start();

	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateOne);

	// first tic does nothing to transition the sate
	m_pTestSM->onTic(); 
	pCureState = m_pTestSM->getCurrentState();

	// we should still be in StateOne
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateOne);

	// second tic does transition in the state's onTic()
	m_pTestSM->onTic(); 
	pCureState = m_pTestSM->getCurrentState();

	// We should now be in stateTwo
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateTwo);

	// first tic does nothing to transition the sate
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// we should still be in StateTwo
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateTwo);

	// second tic does transition in the state's onTic()
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// We should now be in stateThree
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateThree);

	// first tic does nothing to transition the sate
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// we should still be in StateThree
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateThree);

	// second tic does transition in the state's onTic()
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// We should now be in stateDone
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateDone);

	// first tic does nothing to transition the sate
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// we should still be in StateDone
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateDone);

	// second tic does transition in the state's onTic()
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();

	// We should now be in stateIdle
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateIdle);
}