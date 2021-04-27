#ifndef TESTSUBSM_H
#define TESTSUBSM_H

#include "SMBase.h"
#include "TestSMState.h"


class CTestSubEvent : public CSMEvent
{
public:
	enum class EventID
	{
		eNext
	};
	CTestSubEvent(EventID id, std::string name, Priority priority = Priority::Medium)
		: CSMEvent(static_cast<uint32_t>(id), name, priority) {}

	static std::shared_ptr<CTestSubEvent> createEvent(EventID id);
};

class CTestSubEventNext : public CTestSubEvent 
{
public:
	CTestSubEventNext()
		: CTestSubEvent(CTestSubEvent::EventID::eNext, "Sub Event Next") {}
};

class CTestSubState : public CSMBase
{
public:
	enum class StateID {
		eFirst = 0,
		eSubState_1 = eFirst,
		eSubState_2,
		eSubState_3,
		eSubStateCount
	};
	CTestSubState(StateID id, std::string name)
		: CSMBase(static_cast<uint32_t>(id), name) {}

	static std::shared_ptr<CTestSubState> createState(StateID id);
	void addTransition(CTestSubEvent::EventID, StateID);
};

class CTestSubState1 : public CTestSubState
{
public:
	CTestSubState1() : CTestSubState(CTestSubState::StateID::eSubState_1, "Sub State One") {}
	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual void initTransitionTable() override;
};
class CTestSubState2 : public CTestSubState
{
public:
	CTestSubState2() : CTestSubState(CTestSubState::StateID::eSubState_2, "Sub State Two") {}
	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual void initTransitionTable() override;
};
class CTestSubState3 : public CTestSubState
{
public:
	CTestSubState3() : CTestSubState(CTestSubState::StateID::eSubState_3, "Sub State Three") {}
	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual void initTransitionTable() override;
};

class CTestSubContext : public CSMStateContext
{
	bool m_isDone;

public:
	CTestSubContext() : CSMStateContext("Sub Context"), m_isDone(false) {}
	virtual ~CTestSubContext() {}

	void nextState() {
		auto pEvnt = CTestSubEvent::createEvent(CTestSubEvent::EventID::eNext);
		postEvent(pEvnt);
	}

	bool isDone() const { return m_isDone; }
	void setisDone(bool done) { m_isDone = done; }
};


class CTestSubSM 
	: public CSMTestState			
{
public:
	CTestSubSM();
	virtual ~CTestSubSM();

	virtual void initSM();

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

#endif // !TESTSUBSM_H