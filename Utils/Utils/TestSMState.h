#ifndef TESTSMSTATE_H
#define TESTSMSTATE_H

#include "SMState.h"
#include "TestSMEvent.h"

#ifdef _DEBUG 
#define LOG_STATE_ENTER std::cout << "Entering State: " << c_stateName << std::endl
#define LOG_STATE_TICK  std::cout << "Ticking State: " << c_stateName << std::endl
#define LOG_STATE_LEAVE std::cout << "Leaving State: " << c_stateName << std::endl
#else
#define LOG_STATE_ENTER
#define LOG_STATE_TICK
#define LOG_STATE_LEAVE
#endif

class CSMTestState : public CSMState
{
public:
	enum class StateID
	{
		eStateInvalid = -1,
		eStateFirst = 0,

		eStateIdle = eStateFirst,

		eStateOne,
		eStateTwo,
		eStateThree,
		eStateDone,
		eStateError,

		eStateLast = eStateError,
		eStateCount,
	};

	CSMTestState(StateID id, std::string name, bool isFinal = false);
	virtual ~CSMTestState();

	static std::shared_ptr<CSMTestState> createState(StateID id);

protected:
	void addTransition(CTestSMEvent::EventID evnt, CSMTestState::StateID state)
	{
		CSMState::addTransition(static_cast<unsigned int>(evnt), static_cast<unsigned int>(state));
	};
};

class CSMTestStateIdle : public CSMTestState
{
public:
	CSMTestStateIdle()
		: CSMTestState(CSMTestState::StateID::eStateIdle, "State Idle")
	{}
	virtual ~CSMTestStateIdle() {}


	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

class CSMTestStateOne : public CSMTestState
{
public:
	CSMTestStateOne()
		: CSMTestState(CSMTestState::StateID::eStateOne, "State One")
	{}
	virtual ~CSMTestStateOne() {}
	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

class CSMTestStateTwo : public CSMTestState
{
public:
	CSMTestStateTwo()
		: CSMTestState(CSMTestState::StateID::eStateTwo, "State Two")
	{}
	virtual ~CSMTestStateTwo() {}

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

class CSMTestStateThree : public CSMTestState
{
public:
	CSMTestStateThree()
		: CSMTestState(CSMTestState::StateID::eStateThree, "State Three")
	{}
	virtual ~CSMTestStateThree() {}

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

class CSMTestStateDone : public CSMTestState
{
public:
	CSMTestStateDone()
		: CSMTestState(CSMTestState::StateID::eStateDone, "State Done", true)
	{}
	virtual ~CSMTestStateDone() {}

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};

class CSMTestStateError : public CSMTestState
{
public:
	CSMTestStateError()
		: CSMTestState(CSMTestState::StateID::eStateError, "State Error")
	{}
	virtual ~CSMTestStateError() {}

	virtual bool enterState(TContextPtr pCtx, TEventPtr pEvent) override;
	virtual bool ticState(TContextPtr pCtx) override;
	virtual bool exitState(TContextPtr pCtx, TEventPtr pEvent) override;

	virtual void initTransitionTable() override;
};
#endif // !TESTSMSTATE_H