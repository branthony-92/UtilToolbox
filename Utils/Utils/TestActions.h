#ifndef TESTACTION_H
#define TESTACTION_H

#include "AsyncResult.h"
#include "AsyncAction.h"


class TestActionEmpty : public CAsyncAction
{
public:
	TestActionEmpty(TResultPtr pResult)
		: CAsyncAction("ActionEmpty", pResult)
	{}
	void execute() override;
};

class TestActionSimpleAction : public CAsyncAction
{
public:
	TestActionSimpleAction(TResultPtr pResult)
		: CAsyncAction("ActionSimple", pResult)
	{}
	void execute() override;
};

class TestActionComputeFib : public CAsyncAction
{
	std::atomic_bool m_abortFlag;
public:
	TestActionComputeFib(TResultPtr pResult)
		: CAsyncAction("ActionComputeFib", pResult)
		, m_abortFlag(false)
	{}
	void execute() override;
	void abort() override { m_abortFlag.store(true); }

	int computeFib(int nFib);

	struct Params {
		int nFib;
		Params() : nFib(0) {}
	} m_params;
};

class TestActionTimeout : public CAsyncAction
{
	std::atomic_bool m_abortFlag;
public:
	TestActionTimeout(TResultPtr pResult)
		: CAsyncAction("ActionLong", pResult)
		, m_abortFlag(false)
	{}
	void execute() override;
	void abort() override { m_abortFlag.store(true); }
};

class TestActionError : public CAsyncAction
{
public:
	TestActionError(TResultPtr pResult)
		: CAsyncAction("ActionError", pResult)
	{}
	void execute() override;
};

#endif // !TESTACTION_H
