#include "stdafx.h"
#include "AsyncResult.h"

CAsyncResult::CAsyncResult(std::string name)
	: c_name(name)
	, m_status(Status::Idle)
	, m_error("")
{
}

CAsyncResult::CAsyncResult(const CAsyncResult& other)
	: c_name(other.c_name)
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
