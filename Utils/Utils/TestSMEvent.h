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
		eEventNext,
		eEventStop,
		eEventAbort,
		eEventRecover,

		eEventError,
		eEventLast = eEventError,
		eEventCount,
	};

	CTestSMEvent(EventID id, std::string name, CSMEvent::Priority priority = CSMEvent::Priority::Medium);
	~CTestSMEvent();
	
	static TEventPtr createEvent(EventID id);
	
	template <typename eventType> static
	 std::shared_ptr<eventType> getEvent(std::shared_ptr<CTestSMEvent> pEvent) {
		return std::dynamic_pointer_cast<eventType>(pEvent);
	}
};
typedef std::shared_ptr<CTestSMEvent> TTestEventPtr;

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

class CTestSMEventError : public CTestSMEvent
{
public:
	CTestSMEventError()
		: CTestSMEvent(CTestSMEvent::EventID::eEventError, "Event Error")
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