#include "stdafx.h"
#include "AsyncResult.h"

CAsyncResult::CAsyncResult(uint32_t id)
	: c_resultID(id)
	, m_status(Status::Idle)
	, m_error("")
{
}

CAsyncResult::CAsyncResult(const CAsyncResult& other)
	: c_resultID(other.c_resultID)
	, m_status(other.m_status)
	, m_error(other.m_error)
{
}

CAsyncResult& CAsyncResult::operator=(const CAsyncResult& other)
{
	m_status = other.m_status;
	m_error = other.m_error;
	return *this;
}

CAsyncResult::~CAsyncResult()
{
}
