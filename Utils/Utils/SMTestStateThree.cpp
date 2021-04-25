#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateThree::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateThree::ticState(TContextPtr pCtx)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	pContext->step_3();

	return true;
}

bool CSMTestStateThree::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateThree::initTransitionTable()
{
	addTransition( CTestSMEvent::EventID::eEventNext,  CSMTestState::StateID::eStateDone  );
	addTransition( CTestSMEvent::EventID::eEventStop,  CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventAbort, CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventError, CSMTestState::StateID::eStateError );
}
