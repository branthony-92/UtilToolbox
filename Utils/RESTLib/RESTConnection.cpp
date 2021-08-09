#include "StdAfx.h"
#include "RESTConnection.h"


ConnectionManager::Connection::Connection()
	: m_connectionID(0)
	, m_connectionTimeout(0)
	, m_connectionStartTime(std::chrono::system_clock::now())
	, m_connectionToken("")
{
}

ConnectionManager::Connection::Connection(const ConnectionManager::Connection& other)
	: m_connectionID(other.getID())
	, m_connectionTimeout(other.getTimeout())
	, m_connectionStartTime(other.getStartTime())
	, m_connectionToken(other.getToken())
{
}

ConnectionManager::Connection& ConnectionManager::Connection::operator=(const ConnectionManager::Connection& other)
{
	m_connectionID = other.getID();
	m_connectionTimeout = other.getTimeout();
	m_connectionStartTime = other.getStartTime();
	m_connectionToken = other.getToken();

	return *this;
}

bool ConnectionManager::Connection::isTimedOut() const
{
	auto now = std::chrono::system_clock::now();

	auto dt = std::chrono::duration_cast<std::chrono::seconds>(now - m_connectionStartTime).count();

	return dt > m_connectionTimeout;
}

void ConnectionManager::Connection::refreshConnection()
{
	m_connectionStartTime = std::chrono::system_clock::now();
}

bool ConnectionManager::Connection::validate(const unsigned int id, const std::string& token)
{
	if (id != m_connectionID) return false;
	if (m_connectionToken.compare(token) != 0) return false;
	return true;
}
