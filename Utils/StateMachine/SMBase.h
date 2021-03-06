#ifndef SMBASE_H
#define SMBASE_H

#include "SMContext.h"
#include "SMState.h"

class CSMBase : public CSMState
{
public:
	CSMBase(uint32_t id, std::string name, TSMContextPtr pCtx = nullptr);
	~CSMBase();

	void startSM(TStatePtr pInitialState = nullptr);
	void stopSM();
	void resetTimer(unsigned int intervalMS);

	virtual void initSM() {} ;
	virtual void onTic();

protected:
	TStateList       m_stateTable;
	TStatePtr        m_pCurrentState;
	TStatePtr        m_pErrorState;
	TErrorEventPtr   m_pErrorEvent;

	std::list<std::string> m_stateHistory;
	bool m_recordStates;
	bool m_isInitialized;

public:

	TStatePtr getCurrentState() const { return m_pCurrentState; }
	void setCurrentState(TStatePtr pState);

	void setStateTable(TStateList table) { m_stateTable = table; }

	TStatePtr getNextState(uint32_t eventID);

	void setErrorState(TStatePtr pState)  { m_pErrorState = pState; }
	void setErrorEvent(TErrorEventPtr pEvent) { m_pErrorEvent = pEvent; }

	
	void enableStateHistory(bool enabled) { m_recordStates = enabled; }
	std::string dumpStateHistory();

	bool getIsInitialized() const { return m_isInitialized; }

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
	TSMContextPtr m_pContext;

public:
	TSMContextPtr getContext() const { return m_pContext; }
	void setContext(TSMContextPtr pCtx) { m_pContext = pCtx; }
};
typedef std::shared_ptr<CSMBase> TSMPtr;
#endif // !SMBASE_H

