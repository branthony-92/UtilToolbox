#include "stdafx.h"
#include "TestSMState.h"
#include "TestSMEvent.h"
#include "TestContext.h"

bool CSMTestStateIdle::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateIdle::ticState(TContextPtr pCtx)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateIdle::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateIdle::initTransitionTable()
{
	addTransition( CTestSMEvent::EventID::eEventStart,     CSMTestState::StateID::eStateOne );
	addTransition(CTestSMEvent::EventID::eEventStartSubSM, CSMTestState::StateID::eStateSubSM);
	addTransition( CTestSMEvent::EventID::eEventError,     CSMTestState::StateID::eStateError );
}
