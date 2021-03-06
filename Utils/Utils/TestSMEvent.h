#ifndef TESTSMEVENT_H
#define TESTSMEVENT_H

#include "SMEvent.h"

class CTestSMEvent : public CSMEvent
{
public:
	enum class EventID : unsigned int
	{
		eEventInvalid = 0xffffffff,
		eEventFirst = 0,

		eEventStart =  eEventFirst,
		eEventStartSubSM,
		eEventNext,
		eEventStop,
		eEventAbort,
		eEventRecover,
		eEventError,
		eEventLast = eEventError,
		eEventCount,
	};

	CTestSMEvent(EventID id, std::string name, CSMEvent::Priority priority = CSMEvent::Priority::Medium);
	virtual ~CTestSMEvent();
	
	static TEventPtr createEvent(EventID id);
	static TErrorEventPtr createErrorEvent(std::string errMsg = "");
	
	template <typename eventType> static
	 std::shared_ptr<eventType> getEvent(std::shared_ptr<CTestSMEvent> pEvent) {
		return std::dynamic_pointer_cast<eventType>(pEvent);
	}
};
typedef std::shared_ptr<CTestSMEvent> TTestEventPtr;

class CTestSMEventError : public CTestSMEvent, public CSMErrorEvent 
{
public:
	CTestSMEventError()
		: CSMErrorEvent(static_cast<unsigned int>(CTestSMEvent::EventID::eEventError), "Event Error")
		, CTestSMEvent(CTestSMEvent::EventID::eEventError, "Event Error")
	{}
	virtual ~CTestSMEventError() {}
};

class CTestSMEventStartSubSM : public CTestSMEvent
{
public:
	CTestSMEventStartSubSM()
		: CTestSMEvent(CTestSMEvent::EventID::eEventStartSubSM, "Event Start Sub SM")
	{}
};

class CTestSMEventStart : public CTestSMEvent
{
public:
	CTestSMEventStart()
		: CTestSMEvent(CTestSMEvent::EventID::eEventStart, "Event Start")
	{}
};

class CTestSMEventNext : public CTestSMEvent
{
public:
	CTestSMEventNext()
		: CTestSMEvent(CTestSMEvent::EventID::eEventNext, "Event Next")
	{}
};

class CTestSMEventStop : public CTestSMEvent
{
public:
	CTestSMEventStop()
		: CTestSMEvent(CTestSMEvent::EventID::eEventStop, "Event Stop")
	{}
};

class CTestSMEventAbort : public CTestSMEvent
{
public:
	CTestSMEventAbort()
		: CTestSMEvent(CTestSMEvent::EventID::eEventAbort, "Event Abort")
	{}
};


class CTestSMEventRecover : public CTestSMEvent
{
public:
	CTestSMEventRecover()
		: CTestSMEvent(CTestSMEvent::EventID::eEventRecover, "Event Recover")
	{}
};
#endif // !TESTSMEVENT_H