#include "stdafx.h"
#include "TestSM.h"
#include "TestSMState.h"
#include "TestSMEvent.h"

CTestSM::CTestSM(TContextPtr pCtx)
	: CSMBase(-1, "State Test SM", pCtx)
{
}

CTestSM::~CTestSM()
{
}

void CTestSM::initSM()
{
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

	setStateTable(stateTable);

	auto idleIndex   = static_cast<unsigned int>(CSMTestState::StateID::eStateIdle);
	auto errorIndex  = static_cast<unsigned int>(CSMTestState::StateID::eStateError);
	auto pEvent      = CTestSMEvent::createErrorEvent();

	setErrorEvent(pEvent);
	setErrorState(m_stateTable[errorIndex]);
	startSM(m_stateTable[idleIndex]);
}
