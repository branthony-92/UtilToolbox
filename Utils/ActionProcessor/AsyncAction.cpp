#include "stdafx.h"
#include "AsyncAction.h"

using namespace std::chrono_literals;

CAsyncAction::CAsyncAction(std::string name, TResultPtr pResult, uint32_t timeout)
	: m_pResult(pResult)
	, m_pPromise(nullptr)
	, c_timeOut(timeout)
	, c_name(name)
{
	if (!pResult) throw std::runtime_error("Result Not Initialized");
}

CAsyncAction::CAsyncAction(const CAsyncAction& other)
	: m_pResult(other.m_pResult)
	, m_pPromise(other.m_pPromise)
	, c_timeOut(other.c_timeOut)
	, c_name(other.c_name)
{
}

CAsyncAction& CAsyncAction::operator=(const CAsyncAction& other)
{
	m_pResult  = other.m_pResult;
	m_pPromise = other.m_pPromise;
	return *this;
}

CAsyncAction::~CAsyncAction()
{
	abort();
	if (m_pPromise)
	{
		m_pPromise->release();
		m_pPromise = nullptr;
	}
}

CAsyncAction::TPromisePtr CAsyncAction::run()
{
	// create the promise to return 

	// set the result status to executing and launch that thread
	m_pResult->setStatus(CAsyncResult::Status::Executing);
	auto future = std::async(std::launch::async, [&] {
		return runInternal();
	});

	// return the promise containing the shared future
	m_pPromise = std::make_shared<Promise>(c_timeOut, *this);
	m_pPromise->m_future = future.share();

	return m_pPromise;
}

CAsyncResult::Status CAsyncAction::runInternal()
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


CAsyncAction::Promise::Promise(uint32_t timeout, CAsyncAction& action)
	: m_timeout(timeout)
	, m_future()
	, m_action(action)
{
}

CAsyncAction::Promise::~Promise()
{
}

void CAsyncAction::Promise::release()
{
	if (m_future.valid())
	{
		// release the future without caring about the return
		m_action.abort();
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
	m_action.abort();	
	return CAsyncResult::Status::Timeout;
}
