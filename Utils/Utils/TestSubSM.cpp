#include "stdafx.h" 
#include "TestSubSM.h"
#include "TestSMState.h"
CTestSubSM::CTestSubSM(TContextPtr pCtx)
	: CSMBase(pCtx, static_cast<unsigned int>(CSMTestState::StateID::eStateSubSM), "State SubSM")
{

}

CTestSubSM::~CTestSubSM()
{
}

void CTestSubSM::initSM()
{
	// init substates
}

bool CTestSubSM::enterState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

bool CTestSubSM::ticState(TContextPtr pCtx)
{
	onTic();
	return true;
}

bool CTestSubSM::exitState(TContextPtr pCtx, TEventPtr pEvent)
{
	return true;
}

void CTestSubSM::initTransitionTable()
{
	initSM();

}
