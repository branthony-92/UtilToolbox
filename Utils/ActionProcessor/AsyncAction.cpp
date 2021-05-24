#include "stdafx.h"
#include "AsyncAction.h"

using namespace std::chrono_literals;

CAsyncAction::CAsyncAction(uint32_t id, uint32_t timeout, TResultPtr pResult)
	: m_pResult(pResult)
	, c_actionID(id)
	, c_timeOut(timeout)
{
}

CAsyncAction::CAsyncAction(const CAsyncAction& other)
	: m_pResult(other.m_pResult)
	, c_actionID(other.c_actionID)
	, c_timeOut(other.c_timeOut)
{
}

CAsyncAction& CAsyncAction::operator=(const CAsyncAction& other)
{
	m_pResult = other.m_pResult;
	return *this;
}

CAsyncAction::~CAsyncAction()
{
}

CAsyncAction::TPromisePtr CAsyncAction::runActionAsync()
{
	// create the promise to return 

	// set the result status to executing and launch that thread
	m_pResult->setStatus(CAsyncResult::Status::Executing);
	auto future = std::async(std::launch::async, [&] {
		return runActionUnthreaded();
	});

	// return the promise containing the shared future
	TPromisePtr p = std::make_shared<Promise>(c_timeOut);
	p->m_future = future.share();
	return p;
}

CAsyncResult::Status CAsyncAction::runActionUnthreaded()
{
	auto status = CAsyncResult::Status::Error;
	try
	{
		execute();
		status = CAsyncResult::Status::Complete;
	}
	catch (std::exception& er)
	{
		m_pResult->setError(er.what());
	}
	m_pResult->setStatus(status);
	return status;
}


CAsyncAction::Promise::Promise(uint32_t timeout)
	: m_timeout(timeout)
	, m_future()
{
}

CAsyncAction::Promise::~Promise()
{
	release();
}

void CAsyncAction::Promise::release()
{
	if (m_future.valid())
	{
		// release the future without caring about the return
		m_future.wait_for(1ms);
	}
}

CAsyncResult::Status CAsyncAction::Promise::waitForResult() 
{
	if (!m_future.valid()) return CAsyncResult::Status::Invalid;

	std::chrono::duration<int, std::milli> delay(m_timeout);
	auto ret = m_future.wait_for(delay);
	if (ret == std::future_status::ready)
	{
		return m_future.get();
	}
	return CAsyncResult::Status::Timeout;
}
