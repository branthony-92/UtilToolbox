#ifndef SMCONTEXT_H
#define SMCONTEXT_H

#include "SMEvent.h"

typedef std::deque<TEventPtr> TEventQueue;

class CSMStateContext
{
public:
	CSMStateContext(std::string name);
	virtual ~CSMStateContext();

protected:
	TEventQueue m_events;

public:
	virtual void		postEvent(TEventPtr pEvent);
	virtual TEventQueue getEvents() const { return m_events; }
	virtual TEventPtr	getNextEvent();
	virtual void		clearEventQueue();

protected:
	const std::string c_contextName;

public:
	std::string getName() const { return c_contextName; }
};
typedef std::shared_ptr<CSMStateContext> TContextPtr;

#endif // !SMCONTEXT_H
