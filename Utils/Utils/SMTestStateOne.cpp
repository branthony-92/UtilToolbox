#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"

bool CSMTestStateOne::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateOne::ticState(TContextPtr pCtx)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	pContext->step_1();

	return true;
}

bool CSMTestStateOne::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateOne::initTransitionTable()
{
	addTransition( CTestSMEvent::EventID::eEventNext,  CSMTestState::StateID::eStateTwo   );
	addTransition( CTestSMEvent::EventID::eEventStop,  CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventAbort, CSMTestState::StateID::eStateIdle  );
	addTransition( CTestSMEvent::EventID::eEventError, CSMTestState::StateID::eStateError );
}
