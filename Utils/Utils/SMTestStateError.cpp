#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateError::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	//auto pErrorEvent = std::dynamic_pointer_cast<CSMTestStateError>(pEvent);
	//if (!pErrorEvent) return false;

	// do error handling
	

	return true;
}

bool CSMTestStateError::ticState(TContextPtr pCtx)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	// check the context for recovery options
	pContext->recover();

	return true;
}

bool CSMTestStateError::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	// cleanup

	return true;
}

void CSMTestStateError::initTransitionTable()
{
	addTransition(CTestSMEvent::EventID::eEventRecover, CSMTestState::StateID::eStateIdle);
}
