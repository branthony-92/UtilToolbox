#ifndef SMSTATES_H
#define SMSTATES_H

#include "SMContext.h"
#include "SMEvent.h"

typedef std::chrono::high_resolution_clock::time_point  TTimePoint;

typedef std::unordered_map<uint32_t, uint32_t>  TTransitionMap;
typedef std::pair<uint32_t, uint32_t>			TTransitionMapData;

class CSMState
{
public:
	CSMState(uint32_t id, std::string name, bool sFinalState = false);
	virtual ~CSMState();

protected:
	TTransitionMap m_transitions;

public:
	void addTransition(uint32_t eventID, uint32_t targetID);
	uint32_t findTargetState(uint32_t eventID);

public:
	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) { return true; }
	virtual bool ticState(TContextPtr pCtx)						{ return true; }
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent)  { return true; }

	virtual void initTransitionTable() {}

public:
	const std::string  c_stateName;
	const uint32_t c_stateID;

protected:
	std::atomic<bool>  m_isFinalState;

public:
	bool getIsFinalState() const { return m_isFinalState.load(); }

protected:
	TTimePoint m_startTime;
	long	   m_stateTimeout;

public:
	void resetTimer() { m_startTime = std::chrono::high_resolution_clock::now(); }
	TTimePoint getStartTime() const { return m_startTime; }
	bool isTimedOut() const;

	virtual void handleTimeOut() { resetTimer(); } // override this for state-specific timeout handling

protected:
	template <typename context>
	std::shared_ptr<context> recoverCtx(TContextPtr pCtx) {
		return std::dynamic_pointer_cast<context>(pCtx);
	}
};
typedef std::shared_ptr<CSMState>        TStatePtr;
typedef std::map<uint32_t, TStatePtr>    TStateList;

#endif // SMSTATES_H