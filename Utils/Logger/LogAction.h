#ifndef LOGACTION_H
#define LOGACTION_H

#include "AsyncAction.h"
#include "LogMessage.h"

class CLogAction : public CAsyncAction
{
	std::atomic_bool m_abort;

public:
	CLogAction()
		: CAsyncAction("LogAction", std::make_shared<CAsyncResult>("EmptyResult"))
		, m_params()
		, m_abort(false)
	{}

	struct Params {
		std::string logPath;
		TLogQueue   logQueue;
		Params() : logPath(""), logQueue() {}
	} m_params;

protected:
	void execute() override;
	void abort() override { m_abort = true; };
};
typedef std::unique_ptr<CLogAction> TLogActionPtr;

#endif // !LOGACTION_H