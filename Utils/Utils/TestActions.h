#ifndef TESTACTION_H
#define TESTACTION_H

#include "AsyncResult.h"
#include "AsyncAction.h"
#include "ActionContext.h"

class TestActionEmpty : public CAsyncAction
{
public:
	TestActionEmpty(TResultPtr pResult)
		: CAsyncAction("ActionEmpty", pResult)
	{}
	void execute(CActionContext* pCtx) override;
};

class TestActionSimpleAction : public CAsyncAction
{
public:
	TestActionSimpleAction(TResultPtr pResult)
		: CAsyncAction("ActionSimple", pResult)
	{}
	void execute(CActionContext* pCtx) override;
};

class TestActionComputeFib : public CAsyncAction
{
	std::atomic_bool m_abortFlag;
public:
	TestActionComputeFib(TResultPtr pResult)
		: CAsyncAction("ActionComputeFib", pResult)
		, m_abortFlag(false)
	{}
	void execute(CActionContext* pCtx) override;
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
	void execute(CActionContext* pCtx) override;
	void abort() override { m_abortFlag.store(true); }
};

class TestActionError : public CAsyncAction
{
public:
	TestActionError(TResultPtr pResult)
		: CAsyncAction("ActionError", pResult)
	{}
	void execute(CActionContext* pCtx) override;
};

#endif // !TESTACTION_H
