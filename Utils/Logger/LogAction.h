#ifndef LOGACTION_H
#define LOGACTION_H

#include "LogMessage.h"
#include "AsyncAction.h"

class CActionContext;

class CLogAction : public CAsyncAction
{
	std::atomic_bool m_abort;

public:
	CLogAction()
		: CAsyncAction("LogAction", std::make_shared<CAsyncResult>("EmptyResult"))
		, m_abort(false)
	{}

protected:
	void execute(CActionContext* pCtx) override;
	void abort() override { m_abort = true; };
};
typedef std::unique_ptr<CLogAction> TLogActionPtr;

#endif // !LOGACTION_H