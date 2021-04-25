#include "stdafx.h"
#include "TestSM.h"
#include "TestSMState.h"

CTestSM::CTestSM(TContextPtr pCtx)
	: CSMBase(pCtx, -1, "State Test SM")
{
}

CTestSM::~CTestSM()
{
}

void CTestSM::initSM()
{
	auto stateFirst = static_cast<unsigned int>(CSMTestState::StateID::eStateFirst);
	auto stateCount = static_cast<unsigned int>(CSMTestState::StateID::eStateCount);

	// init our state list and transition tables
	for (auto i = stateFirst; i < stateCount; i++)
	{
		auto id = static_cast<CSMTestState::StateID>(i);
		auto pState = CSMTestState::createState(id);	

		if (pState)
		{
			pState->initTransitionTable();
			m_stateTable.push_back(pState);
		}
	}

	auto idleIndex = static_cast<unsigned int>(CSMTestState::StateID::eStateIdle);
	auto errorIndex = static_cast<unsigned int>(CSMTestState::StateID::eStateError);

	setErrorState(m_stateTable[errorIndex]);
	startSM(m_stateTable[idleIndex]);
}
