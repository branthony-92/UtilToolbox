#ifndef TESTCONTEXT_H
#define TESTCONTEXT_H

#include "TestSMState.h"
#include "TestSM.h"

typedef std::deque<CSMTestState::StateID> TStateIDQueue;

class CTestContext 
	: public CSMStateContext
{

public:
	CTestContext();
	~CTestContext();

public:

private:
	// state machine
	TTestSMPtr m_pSM;

private:
	// state tracking
	TStateIDQueue m_stateHistory;

public:
	// state tracking
	void addStateID(CSMTestState::StateID stateID);
	TStateIDQueue& getStateHistory() { return m_stateHistory; }
	CSMTestState::StateID getCurrentState() const;

	void start();

	void step_1();
	void step_2();
	void step_3();

	void stop();

	void abort();

	std::atomic<bool> m_testDone;

	bool getTestDone() const { return m_testDone.load(); }
	void setTestDone(bool done) { m_testDone.store(done); }

};
typedef std::shared_ptr<CTestContext> TTestContextPtr;

#endif // !TESTCONTEXT_H
