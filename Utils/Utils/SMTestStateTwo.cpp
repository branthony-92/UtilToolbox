#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateTwo::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateTwo::ticState(TContextPtr pCtx)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	pContext->step_2();

	return true;
}

bool CSMTestStateTwo::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateTwo::initTransitionTable()
{
	addTransition( CTestSMEvent::EventID::eEventNext,  CSMTestState::StateID::eStateThree  );
	addTransition( CTestSMEvent::EventID::eEventStop,  CSMTestState::StateID::eStateIdle   );
	addTransition( CTestSMEvent::EventID::eEventAbort, CSMTestState::StateID::eStateIdle   );
	addTransition( CTestSMEvent::EventID::eEventError, CSMTestState::StateID::eStateError  );
}
