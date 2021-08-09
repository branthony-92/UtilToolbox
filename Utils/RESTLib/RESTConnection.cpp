#include "StdAfx.h"
#include "RESTConnection.h"

#include <random>

ConnectionManager::ConnectionManager()
	: m_connections()
{
}

ConnectionManager::~ConnectionManager()
{
}


ConnectionManager::Token ConnectionManager::refreshConnection(const unsigned int id)
{
	auto pConnection = getConnection(id);
	if (!pConnection) return Token();
	
	auto& token = pConnection->tokenRef();

	token.startTime = std::chrono::system_clock::now();
	token.val = generateTokenValue();
	pConnection->setStatus(ConnectionStatus::Open);

	return token;
}

void ConnectionManager::removeClosedConnections()
{
	for (auto iter = m_connections.begin(); iter != m_connections.end(); iter++)
	{
		auto pConnection = iter->second;
		if (pConnection->getStatus() == ConnectionStatus::Closed)
		{
			iter = m_connections.erase(iter);
		}
	}
}

unsigned int ConnectionManager::generateID()
{
	std::random_device gen;
	auto id = gen();
	return id;
}

std::string ConnectionManager::generateTokenValue()
{
	std::string tokenString = "";
	return tokenString;
}

ConnectionManager::ConnectionStatus ConnectionManager::validateConnection(const unsigned int id, const std::string& token)
{
	auto pConnection = getConnection(id);
	if (!pConnection) return ConnectionStatus::Invalid;

	// check the ID
	if (pConnection->getID() != id)
	{
		pConnection->setStatus(ConnectionStatus::Invalid);
		m_connections.erase(m_connections.find(id));
		return ConnectionStatus::Invalid;
	}

	// check the age
	if (pConnection->isTimedOut())
	{
		pConnection->setStatus(ConnectionStatus::StaleToken);
	}

	// check the token
	if (!pConnection->getToken().compare(token))
	{
		pConnection->setStatus(ConnectionStatus::BadToken);
	}

	return pConnection->getStatus();
}


void ConnectionManager::closeConnection(const unsigned int id)
{
	auto pConnection = getConnection(id);
	if (!pConnection) return;

	pConnection->setStatus(ConnectionStatus::Closed);
}

ConnectionManager::TConnectionPtr ConnectionManager::openNewConnection(Token token)
{
	auto connectionID = generateID();

	// check if this ID is already in use
	TConnectionPtr pConnection = getConnection(connectionID);
	while (pConnection != nullptr)
	{
		// generate a new ID until we get a new one
		connectionID = generateID();
		pConnection = getConnection(connectionID);
	}

	pConnection = std::make_shared<Connection>(connectionID);

	// generate the token value
	token.val = generateTokenValue();
	pConnection->setToken(token);

	m_connections.insert_or_assign(connectionID, pConnection);
	return pConnection;
}

ConnectionManager::TConnectionPtr ConnectionManager::getConnection(unsigned int id)
{
	TConnectionPtr pConnection = nullptr;
	auto connectionIter = m_connections.find(id);

	if (connectionIter != m_connections.end())
	{
		pConnection = m_connections[id];
	}
	return pConnection;
}

bool ConnectionManager::Connection::isTimedOut() const
{
	auto now = std::chrono::system_clock::now();
	auto dt = std::chrono::duration_cast<std::chrono::seconds>(now - m_connectionToken.startTime).count();
	return dt > m_connectionToken.timeout;	
}