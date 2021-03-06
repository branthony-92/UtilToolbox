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
	std::string m_lastError;
	std::string m_resourceRoot;
public:
	std::string getCtxName() const { return c_contextName; }

	std::string getLastError() const { return m_lastError; }
	void setLastError(std::string err) { m_lastError = err; }

	void setResourceRootDirectory(std::string dir) { m_resourceRoot = dir; }
};
typedef std::shared_ptr<CSMStateContext> TSMContextPtr;

#endif // !SMCONTEXT_H
