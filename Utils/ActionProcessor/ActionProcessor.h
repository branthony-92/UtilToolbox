#ifndef ACTIONPROCESSOR_H
#define ACTIONPROICESOR_H

#include "AsyncAction.h"

typedef std::lock_guard<std::mutex> TLock;

class CActionProcessor
{
public:
	class Promise;

	typedef std::shared_ptr<Promise> TPromisePtr;
	typedef std::vector<TPromisePtr> TPromiseList;

	CActionProcessor(uint32_t timeoutMS = 1000u);
	virtual ~CActionProcessor();

	TPromisePtr pushAction(TActionPtr pAction);
	bool eraseAction(TActionPtr pAction);
	void purgeActions();

	void beginProcessing();
	void stopProcessing();
	
private:
	TActionPtr getNextAction();

	std::deque<TActionPtr> m_actionQueue;

	std::thread		   m_actionProcThread;
	mutable std::mutex m_mutex;
	
	std::atomic_bool m_processing;
	std::atomic_uint m_timeOutMS;

	TPromiseList m_promises;
};
typedef std::shared_ptr<CActionProcessor> TActionProcPtr;

class CActionProcessor::Promise 
{
public:
	Promise(uint32_t timeout)
		: m_timeout(timeout)
		, m_future()
	{
	}
	~Promise() {}

	TResultPtr getResult() {
		std::chrono::duration<int, std::milli> delay(m_timeout);
		auto ret = m_future.wait_for(delay);
		if (ret == std::future_status::ready)
		{
			return m_future.get();
		}
		return nullptr;
	}

	void setFuture(std::shared_future<TResultPtr> future) { m_future = future; }

private:
	uint32_t m_timeout;
	std::shared_future<TResultPtr> m_future;
};

namespace {
	const uint32_t c_defaultWorkerCount = 4u;
}
class CActionProcessorPool
{
public:
	CActionProcessorPool(uint32_t timeoutMS , uint32_t numWorkers);
	virtual ~CActionProcessorPool();

	TActionProcPtr delegateAction(TActionPtr pAction);
	TActionProcPtr getProccessor(uint32_t index);

private:
	TActionProcPtr getNexProc();

private:
	typedef std::vector<TActionProcPtr>  TProcList;

	TProcList m_procPool;
	uint32_t  m_nextProc;
};
std::unique_ptr<CActionProcessor> TActionProcPoolPtr;
#endif // !ACTIONPROCESSOR_H