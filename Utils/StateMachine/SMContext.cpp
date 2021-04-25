#include "stdafx.h"
#include "SMContext.h"

CSMStateContext::CSMStateContext(std::string name)
	: c_contextName(name)
	, m_events()
	, m_lastError()
{
}

CSMStateContext::~CSMStateContext()
{
}


TEventPtr CSMStateContext::getNextEvent()
{
	TEventPtr pEvent = nullptr;
	if (m_events.empty()) return pEvent;

	pEvent = m_events.front();
	m_events.pop_front();

	return pEvent;
}

void CSMStateContext::postEvent(TEventPtr pEvent)
{
	if (!pEvent) return;

	switch (pEvent->c_eventPriority)
	{
	case CSMEvent::Priority::Critical: // critical events warrent clearing all other events (eg abort/error)
		clearEventQueue();
		m_events.push_front(pEvent);
		break;
	case CSMEvent::Priority::High:    // high priority events will be pushed to the front
		m_events.push_front(pEvent);
		break;
	case CSMEvent::Priority::Medium:  // Low/medium priority are just added to the queue and are currently
		m_events.push_back(pEvent);
		break;
	case CSMEvent::Priority::Low:
		m_events.push_back(pEvent);
		break;
	default:
		break;
	}
}

void CSMStateContext::clearEventQueue()
{
	m_events.clear();
}
