#include "stdafx.h"
#include "TestContext.h"

namespace Constants {
	constexpr unsigned int c_historyLimit = 50; // how many of our states do we need to remember at any given time
}

CTestContext::CTestContext()
	: CSMStateContext("Test Context")
	, m_pSM(nullptr)
	, m_testDone(false)
{
}

CTestContext::~CTestContext()
{
}

void CTestContext::addStateID(CSMTestState::StateID stateID)
{
	m_stateHistory.push_back(stateID);
	if (m_stateHistory.size() > Constants::c_historyLimit)
	{
		// start treadmilling the state history
		m_stateHistory.pop_front();
	}
}

CSMTestState::StateID CTestContext::getCurrentState() const
{
	if (m_stateHistory.empty()) return CSMTestState::StateID::eStateInvalid;
	return m_stateHistory.back();
}

void CTestContext::start()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventStart);
	postEvent(pEvent);
}

void CTestContext::startSubSM()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventStartSubSM);
	postEvent(pEvent);
}

void CTestContext::next()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventNext);
	postEvent(pEvent);
}

void CTestContext::step_1()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventNext);
	postEvent(pEvent);
}

void CTestContext::step_2()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventNext);
	postEvent(pEvent);
}

void CTestContext::step_3()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventNext);
	postEvent(pEvent);
}

void CTestContext::stop()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventStop);
	postEvent(pEvent);
	setTestDone(true);
}

void CTestContext::abort()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventAbort);
	postEvent(pEvent);
}

void CTestContext::recover()
{
	auto pEvent = CTestSMEvent::createEvent(CTestSMEvent::EventID::eEventRecover);
	postEvent(pEvent);
}

void CTestContext::logLastError()
{
	// std::cout << m_lastError << "/n";
}