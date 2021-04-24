#ifndef SMBASE_H
#define SMBASE_H

#include "SMContext.h"
#include "SMState.h"

class CSMBase : public CSMState
{
public:
	CSMBase(TContextPtr pCtx, uint32_t id, std::string name);
	~CSMBase();

	void startSM(TStatePtr pInitialState = nullptr);
	virtual void onTic();

protected:
	TStateList  m_stateTable;
	TStatePtr   m_pCurrentState;
	TStatePtr   m_pErrorState;

public:

	void setNextState(TStatePtr pState);
	TStatePtr getNextState(uint32_t eventID);

	void setErrorState(TStatePtr pState)  { m_pErrorState = pState; }
	void setStateTable(TStateList states) { m_stateTable = states; }

protected:
	std::thread					m_ticThread;
	std::atomic<uint32_t>   m_ticIntervalMS;
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
};
#endif // !SMBASE_H

