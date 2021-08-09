#ifndef ASYNCACTION_H
#define ASYNCACTION_H

#include "AsyncResult.h"
class CActionContext;

class CAsyncAction
{
public:
	class Promise;
	typedef std::shared_ptr<Promise> TPromisePtr;
	typedef std::shared_future<CAsyncResult::Status> TActionFuture;

	CAsyncAction(std::string name, TResultPtr pResult, uint32_t timeout = 1000u);
	CAsyncAction(const CAsyncAction& other);
	CAsyncAction& operator=(const CAsyncAction& other);
	virtual ~CAsyncAction();

	TPromisePtr run(CActionContext* pCtx);

	TResultPtr getResult() const { return m_pResult; }
	TPromisePtr getPromise() const { return m_pPromise; }

	// may as well make these public since they're const
	const uint32_t    c_timeOut;  
	const std::string c_name;

protected:
	CAsyncResult::Status  runInternal(CActionContext* pCtx);
	
	// override this to provide the core execution logic for the derived action
	virtual void execute(CActionContext* pCtx) = 0;
	
	// override this to provide the option to abort potentially long or expensive derived action
	virtual void abort() { /* do nothing by default*/ } 

	TResultPtr      m_pResult;
	TPromisePtr     m_pPromise;
};
typedef std::shared_ptr<CAsyncAction> TActionPtr;

class CAsyncAction::Promise
{
public:
	Promise(uint32_t timeout, CAsyncAction& action);
	~Promise();

	//used to 
	friend CAsyncAction; 

	CAsyncResult::Status waitForResult();

	void release();

private:
	uint32_t	  m_timeout;
	TActionFuture m_future;
	CAsyncAction& m_action;
};
#endif // !ASYNCACTION_H