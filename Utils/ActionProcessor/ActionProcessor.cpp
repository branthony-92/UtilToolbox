#include "stdafx.h"
#include "ActionProcessor.h"

CActionProcessor::CActionProcessor(uint32_t timeoutMS)
	: m_timeOutMS(timeoutMS)
{

}

CActionProcessor::~CActionProcessor()
{

}

CActionProcessor::TPromisePtr CActionProcessor::pushAction(TActionPtr pAction)
{
	TPromisePtr p = std::make_shared<Promise>(pAction->c_timeOut);
	auto future = std::async(std::launch::async, [&] {
		pAction->execAsync();
		return pAction->getResult();
	});

	p->setFuture(future.share());
	return p;
}


void CActionProcessor::purgeActions()
{
	TLock lock(m_mutex);
	m_actionQueue.clear();
}

void CActionProcessor::beginProcessing()
{
	if (m_processing.load()) return;
	m_processing = true;

	m_actionProcThread = std::thread([&] {
	
		while (m_processing.load())
		{
			std::chrono::duration<int, std::milli> delay(m_timeOutMS.load());
			std::this_thread::sleep_for(delay);
			auto pAction = getNextAction();
			if (pAction) pAction->execAsync();
		}
	});
}

void CActionProcessor::stopProcessing()
{
	m_processing = false;
	if (m_actionProcThread.joinable())
	{
		m_actionProcThread.join();
	}
}

TActionPtr CActionProcessor::getNextAction()
{
	TLock lock(m_mutex);
	if (m_actionQueue.empty()) return nullptr;
	auto pAction = m_actionQueue.front();
	m_actionQueue.pop_front();

	return pAction;
}


CActionProcessorPool::CActionProcessorPool(uint32_t timeoutMS, uint32_t numWorkers)
	: m_procPool()
	, m_nextProc(0)
{
	if (numWorkers < 1) numWorkers = 1;
	for (auto i = 0u; i < numWorkers; i++)
	{
		auto pProc = std::make_shared<CActionProcessor>(timeoutMS);
		m_procPool.push_back(pProc);
	}
}

CActionProcessorPool::~CActionProcessorPool()
{
	for (auto p : m_procPool)
	{
		p->stopProcessing();
	}
}

TActionProcPtr CActionProcessorPool::delegateAction(TActionPtr pAction)
{
	auto pProccessor = getNexProc();
	if (pProccessor) pProccessor->pushAction(pAction);
	return pProccessor;
}

TActionProcPtr CActionProcessorPool::getProccessor(uint32_t index)
{
	if (index >= m_procPool.size()) return nullptr;
	return m_procPool[index];
}


TActionProcPtr CActionProcessorPool::getNexProc()
{
	if (m_procPool.empty()) return nullptr;
	if (m_procPool.size() == 1) return m_procPool[0]; // return the only one

	auto pProcessor = m_procPool[m_nextProc++];
	if (m_nextProc == m_procPool.size()) m_nextProc = 0;
	return pProcessor;
}
