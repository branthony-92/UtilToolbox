#include "stdafx.h" 
#include "TestSubSM.h"
#include "TestSMState.h"
#include "TestContext.h"

CTestSubSM::CTestSubSM()
	: CSMTestState(CSMTestState::StateID::eStateSubSM, "State SubSM")
{

}

CTestSubSM::~CTestSubSM()
{
}

void CTestSubSM::initSM()
{
	// init this SM's substates
	// here we can create a context
	auto pCtx = std::make_shared<CTestSubContext>();
	setContext(pCtx);

	auto stateFirst = static_cast<unsigned int>(CTestSubState::StateID::eFirst);
	auto stateCount = static_cast<unsigned int>(CTestSubState::StateID::eSubStateCount);

	TStateList stateTable;

	// init our state list and transition tables
	for (auto i = stateFirst; i < stateCount; i++)
	{
		auto id = static_cast<CTestSubState::StateID>(i);
		auto pState = CTestSubState::createState(id);

		if (pState)
		{
			pState->initTransitionTable();
			stateTable.push_back(pState);
		}
	}

	setStateTable(stateTable);

	auto idleIndex = static_cast<unsigned int>(CTestSubState::StateID::eSubState_1);
	auto errorIndex = static_cast<unsigned int>(CTestSubState::StateID::eSubState_1);
	auto pEvent = CTestSMEvent::createErrorEvent();

	setErrorEvent(pEvent);
	setCurrentState(m_stateTable[idleIndex]);
	setErrorState(m_stateTable[errorIndex]);
}

bool CTestSubSM::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	initSM();
	return true;
}

bool CTestSubSM::ticState(TContextPtr pCtx)
{
	auto pParentCtx = recoverCtx<CTestContext>(pCtx);
	auto pChildCtx = recoverCtx<CTestSubContext>(m_pContext);

	onTic();

	if (pChildCtx->isDone())
	{
		pParentCtx->stop();
	}

	return true;
}

bool CTestSubSM::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void CTestSubSM::initTransitionTable()
{
	addTransition(CTestSMEvent::EventID::eEventStop,  CSMTestState::StateID::eStateIdle);
	addTransition(CTestSMEvent::EventID::eEventAbort, CSMTestState::StateID::eStateIdle);
	addTransition(CTestSMEvent::EventID::eEventError, CSMTestState::StateID::eStateError);
}

bool CTestSubState1::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pSubCtx = recoverCtx<CTestSubContext>(pCtx);
	pSubCtx->setisDone(false);
	return true;
}

bool CTestSubState1::ticState(TContextPtr pCtx)
{
	auto pSubCtx = recoverCtx<CTestSubContext>(pCtx);
	pSubCtx->nextState();
	return true;
}

bool CTestSubState1::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void CTestSubState1::initTransitionTable()
{
	addTransition(CTestSubEvent::EventID::eNext, CTestSubState::StateID::eSubState_2);
}

bool CTestSubState2::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool CTestSubState2::ticState(TContextPtr pCtx)
{
	auto pSubCtx = recoverCtx<CTestSubContext>(pCtx);
	pSubCtx->nextState();
	return true;
}

bool CTestSubState2::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void CTestSubState2::initTransitionTable()
{
	addTransition(CTestSubEvent::EventID::eNext, CTestSubState::StateID::eSubState_3);
}

bool CTestSubState3::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool CTestSubState3::ticState(TContextPtr pCtx)
{
	auto pSubCtx = recoverCtx<CTestSubContext>(pCtx);
	pSubCtx->setisDone(true);
	return true;
}

bool CTestSubState3::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void CTestSubState3::initTransitionTable()
{
	addTransition(CTestSubEvent::EventID::eNext, CTestSubState::StateID::eSubState_3);
}

std::shared_ptr<CTestSubEvent> CTestSubEvent::createEvent(EventID id)
{
	TEventPtr pEvent = nullptr;
	switch (id)
	{
	case EventID::eNext:
		return std::make_shared< CTestSubEventNext>();
		break;
	default:
		break;
	}
}

std::shared_ptr<CTestSubState> CTestSubState::createState(StateID id)
{
	std::shared_ptr<CTestSubState> pState = nullptr;

	switch (id)
	{
	case CTestSubState::StateID::eSubState_1:
		pState = std::make_shared<CTestSubState1>();
		break;
	case CTestSubState::StateID::eSubState_2:
		pState = std::make_shared<CTestSubState2>();
		break;
	case CTestSubState::StateID::eSubState_3:
		pState = std::make_shared<CTestSubState3>();
		break;
	default:
		break;
	}

	return pState;
}

void CTestSubState::addTransition(CTestSubEvent::EventID eventID, StateID targetState)
{
	CSMState::addTransition(static_cast<uint32_t>(eventID), static_cast<uint32_t>(targetState));
}
