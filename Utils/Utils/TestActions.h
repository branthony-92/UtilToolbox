#ifndef TESTACTION_H
#define TESTACTION_H

#include "AsyncResult.h"
#include "AsyncAction.h"

enum class ActionID
{
	ActionInvalid = -1,

	ActionFirst = 0,
	ActionEmpty = ActionFirst,
	ActionSimple,
	ActionLong,
	ActionError,

	NumActions,
};

class TestActionEmpty : public CAsyncAction
{
public:
	TestActionEmpty(TResultPtr pResult)
		: CAsyncAction(static_cast<uint32_t>(ActionID::ActionEmpty), 1000u, pResult)
	{}
	void execute() override;
};

class TestActionSimpleAction : public CAsyncAction
{
public:
	TestActionSimpleAction(TResultPtr pResult)
		: CAsyncAction(static_cast<uint32_t>(ActionID::ActionSimple), 1000u, pResult)
	{}
	void execute() override;
};

class TestActionTimeout : public CAsyncAction
{
	std::atomic_bool m_abortFlag;
public:
	TestActionTimeout(TResultPtr pResult)
		: CAsyncAction(static_cast<uint32_t>(ActionID::ActionLong), 1000u, pResult)
		, m_abortFlag(false)
	{}
	void execute() override;
	void abort() override { m_abortFlag.store(true); }
};

class TestActionError : public CAsyncAction
{
public:
	TestActionError(TResultPtr pResult)
		: CAsyncAction(static_cast<uint32_t>(ActionID::ActionError), 1000u, pResult)
	{}
	void execute() override;
};

#endif // !TESTACTION_H
