#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateError::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	auto pErrorEvent = std::dynamic_pointer_cast<CSMErrorEvent>(pEvent);
	if (!pErrorEvent) return false;

	// log the last error
	pContext->setLastError(pErrorEvent->getErrorMsg());
	pContext->logLastError();

	return true;
}

bool CSMTestStateError::ticState(TContextPtr pCtx)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateError::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = recoverCtx<CTestContext>(pCtx);
	if (!pContext) return false;

	// cleanup

	return true;
}

void CSMTestStateError::initTransitionTable()
{
	addTransition(CTestSMEvent::EventID::eEventRecover, CSMTestState::StateID::eStateIdle);
}
