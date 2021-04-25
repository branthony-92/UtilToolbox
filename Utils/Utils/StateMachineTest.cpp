#include "stdafx.h"
#include "StateMachineTest.h"
#include "TestSMState.h"
void StateMachineTest::SetUp()
{
	m_pTestCtx = std::make_shared<CTestContext>();
	m_pTestSM  = std::make_unique<CTestSM>(m_pTestCtx);
	m_pInitialState = std::make_shared<CSMTestStateIdle>();
	
	EXPECT_TRUE(m_pTestCtx != nullptr);
	EXPECT_TRUE(m_pTestSM != nullptr);
	EXPECT_TRUE(m_pInitialState != nullptr);

	auto stateFirst = static_cast<unsigned int>(CSMTestState::StateID::eStateFirst);
	auto stateCount = static_cast<unsigned int>(CSMTestState::StateID::eStateCount);
	TStateList stateTable;

	// init our state list and transition tables
	for (auto i = stateFirst; i < stateCount; i++)
	{
		auto id = static_cast<CSMTestState::StateID>(i);
		auto pState = CSMTestState::createState(id);

		// ensure all of our states are being created and added to the table
		EXPECT_TRUE(pState != nullptr);

		if (pState)
		{
			pState->initTransitionTable();
			stateTable.push_back(pState);
		}
	}
	// our state table should have the correct number of states
	EXPECT_EQ(stateTable.size(), stateCount);

	auto idleIndex = static_cast<unsigned int>(CSMTestState::StateID::eStateIdle);
	auto pIdleState = std::dynamic_pointer_cast<CSMTestStateIdle>(stateTable[idleIndex]);

	// Our idle state needs to exist
	EXPECT_TRUE(pIdleState != nullptr);
	m_pInitialState = pIdleState;

	// Our error event needs to exist
	auto pEvent = CTestSMEvent::createErrorEvent();
	EXPECT_TRUE(pEvent != nullptr);
	m_pTestSM->setErrorEvent(pEvent);

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

TEST_F(StateMachineTest, TestBadStateTransitions)
{
	m_pTestSM->setCurrentState(m_pInitialState);
	auto pCureState = m_pTestSM->getCurrentState();

	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateIdle);

	// can't transition from with "Next" event
	m_pTestCtx->next();
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();
	EXPECT_TRUE(pCureState != nullptr);

	// we should still be in the original state when we transition after the first tic
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateIdle);

	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();
	EXPECT_TRUE(pCureState != nullptr);

	// we should now be in the error state after the second tic
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateError);

	// check that we have an error message stored in the context
	auto lastErr = m_pTestCtx->getLastError();
	EXPECT_FALSE(lastErr.empty());

	for (auto i = 0; i < 5; i++)
	{
		m_pTestSM->onTic();
		pCureState = m_pTestSM->getCurrentState();
		EXPECT_TRUE(pCureState != nullptr);

		// we should now be in the error state after the second tic
		EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateError);
	}

	// attempt to recover from the error state
	m_pTestCtx->recover();
	m_pTestSM->onTic();
	pCureState = m_pTestSM->getCurrentState();
	EXPECT_TRUE(pCureState != nullptr);

	// we should now be back in the idle state after the tic
	EXPECT_EQ(static_cast<CSMTestState::StateID>(pCureState->c_stateID), CSMTestState::StateID::eStateIdle);
}

TEST_F(StateMachineTest, TestStateMachineStateControl)
{
	m_pTestSM->setCurrentState(m_pInitialState);
	auto pCureState = m_pTestSM->getCurrentState();

	// idle->state_1->state_2->state_3->done->idle

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