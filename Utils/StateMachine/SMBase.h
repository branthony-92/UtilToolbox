#ifndef SMBASE_H
#define SMBASE_H

#include "SMContext.h"
#include "SMState.h"

class CSMBase : public CSMState
{
public:
	CSMBase(uint32_t id, std::string name, TContextPtr pCtx = nullptr);
	~CSMBase();

	void startSM(TStatePtr pInitialState = nullptr);
	void stopSM();
	void resetTimer(unsigned int intervalMS);
	virtual void onTic();

protected:
	TStateList       m_stateTable;
	TStatePtr        m_pCurrentState;
	TStatePtr        m_pErrorState;
	TErrorEventPtr   m_pErrorEvent;

public:

	TStatePtr getCurrentState() const { return m_pCurrentState; }
	void setCurrentState(TStatePtr pState) { m_pCurrentState = pState; }

	void setStateTable(TStateList table) { m_stateTable = table; }

	void setNextState(TStatePtr pState);
	TStatePtr getNextState(uint32_t eventID);

	void setErrorState(TStatePtr pState)  { m_pErrorState = pState; }
	void setErrorEvent(TErrorEventPtr pEvent) { m_pErrorEvent = pEvent; }

protected:
	std::thread					m_ticThread;
	std::atomic<uint32_t>		m_ticIntervalMS;
	std::atomic<bool>			m_smEnabled;
	std::atomic<bool>			m_finalStateReached;

public:
	bool getSMEnabled() const { return m_smEnabled.load(); }
	void setSMEnabled(bool enabled) { m_smEnabled.store(enabled); }

	uint32_t getTicInterval() const { return m_ticIntervalMS.load(); }
	void setTicInterval(uint32_t intervalMS) { m_ticIntervalMS.store(intervalMS); }

	bool getSMDone() const { return m_finalStateReached.load(); }
	void setSMDone(bool done) { m_finalStateReached = done; }

protected:
	TContextPtr m_pContext;

public:
	TContextPtr getContext() const { return m_pContext; }
	void setContext(TContextPtr pCtx) { m_pContext = pCtx; }
};
typedef std::shared_ptr<CSMBase> TSMPtr;
#endif // !SMBASE_H

