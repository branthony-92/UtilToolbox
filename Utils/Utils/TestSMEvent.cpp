#include "stdafx.h"
#include "TestSMEvent.h"

CTestSMEvent::CTestSMEvent(EventID id, std::string name, CSMEvent::Priority priority)
	: CSMEvent(static_cast<unsigned int>(id), name, priority)
{
}

CTestSMEvent::~CTestSMEvent()
{
}

TEventPtr CTestSMEvent::createEvent(EventID id)
{
	TEventPtr pEvent = nullptr;
	switch (id)
	{
	case CTestSMEvent::EventID::eEventStartSubSM:
		pEvent = std::make_shared<CTestSMEventStartSubSM>();
		break;
	case CTestSMEvent::EventID::eEventStart:
		pEvent = std::make_shared<CTestSMEventStart>();
		break;
	case CTestSMEvent::EventID::eEventNext:
		pEvent = std::make_shared<CTestSMEventNext>();
		break;
	case CTestSMEvent::EventID::eEventStop:
		pEvent = std::make_shared<CTestSMEventStop>();
		break;
	case CTestSMEvent::EventID::eEventAbort:
		pEvent = std::make_shared<CTestSMEventAbort>();
		break;
	case CTestSMEvent::EventID::eEventRecover:
		pEvent = std::make_shared<CTestSMEventRecover>();
		break;
	default:
		break;
	}

	return pEvent;
}

TErrorEventPtr CTestSMEvent::createErrorEvent(std::string errMsg)
{
	auto pEvent = std::make_shared<CTestSMEventError>();
	pEvent->setErrorMsg(errMsg);
	return pEvent;
}
