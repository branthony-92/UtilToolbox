#include "stdafx.h"
#include "TestSMState.h"
#include "TestContext.h"


bool CSMTestStateError::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;


	return true;
}

bool CSMTestStateError::ticState(TContextPtr pCtx)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

bool CSMTestStateError::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	auto pContext = getContext<CTestContext>(pCtx);
	if (!pContext) return false;

	return true;
}

void CSMTestStateError::initTransitionTable()
{

}
