#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateDone::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateDone::ticState(TContextPtr pCtx)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	pContext->stop();

	return true;
}

bool CSMTestStateDone::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateDone::initTransitionTable()
{
	addTransition( CTestSMEvent::EventID::eEventNext,  CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventStop,  CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventAbort, CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventError, CSMTestState::StateID::eStateError );
}
