#ifndef ASYNCACTION_H
#define ASYNCACTION_H

#include "AsyncResult.h"

class CAsyncAction
{
public:
	class Promise;
	typedef std::shared_ptr<Promise> TPromisePtr;

	CAsyncAction(uint32_t id, uint32_t timeout, TResultPtr pResult);
	CAsyncAction(const CAsyncAction& other);
	CAsyncAction& operator=(const CAsyncAction& other);
	virtual ~CAsyncAction();

	TPromisePtr runActionAsync();
	CAsyncResult::Status  runActionUnthreaded();

	TResultPtr getResult() const       { return m_pResult; }
	void setResult(TResultPtr pResult) { m_pResult = pResult; }

	// may as well make these public since they're const
	const uint32_t c_actionID;
	const uint32_t c_timeOut;  

protected:
	
	// override this to provide the core execution logic for the derived action
	virtual void execute() = 0;
	
	// override this to provide the option to abort potentially long or expensive derived action
	virtual void abort() { /* do nothing by default*/ } 
	TResultPtr m_pResult;
};
typedef std::shared_ptr<CAsyncAction> TActionPtr;

class CAsyncAction::Promise
{
public:
	Promise(uint32_t timeout);
	~Promise();

	// we live inside this class so it's only fair
	friend CAsyncAction; 

	CAsyncResult::Status waitForResult();

	void release();

private:
	uint32_t m_timeout;
	std::shared_future<CAsyncResult::Status> m_future;
};
#endif // !ASYNCACTION_H