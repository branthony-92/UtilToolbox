#ifndef RESTCONNECTION_H
#define RESTCONNECTION_H

class Connection;

typedef std::chrono::system_clock::time_point   TimeStamp;
typedef std::shared_ptr<Connection>             TConnectionPtr;
typedef std::map<unsigned int, TConnectionPtr>  TConnectionMap;
typedef std::pair<unsigned int, TConnectionPtr> TConnectionMapData;

class ConnectionManager {
public:
	class Connection;

	ConnectionManager();
	virtual ~ConnectionManager();

	// connection management
	TConnectionPtr openConnection();
	void closeConnection(const unsigned int id);
	bool validateConnection(const unsigned int id);
	bool refreshConnection(const unsigned int id);
	TConnectionPtr getConnection(unsigned int id);

	TConnectionMap m_openConnections;
};

class ConnectionManager::Connection
{
public:

	Connection();
	Connection(const Connection& other);
	Connection& operator=(const Connection& other);

	virtual ~Connection() {}

	bool isTimedOut() const;
	void refreshConnection();
	bool validate(const unsigned int id, const std::string& token);

public:
	unsigned int getID()         const { return m_connectionID; }
	unsigned int getTimeout()    const { return m_connectionTimeout; }
	TimeStamp    getStartTime()  const { return m_connectionStartTime; }
	std::string  getToken()      const { return m_connectionToken; }

	void setID(unsigned int id) { m_connectionID = id; }
	void setTimeout(unsigned int timeout) { m_connectionTimeout = timeout; }
	void setStartTime(TimeStamp startTime) { m_connectionStartTime = startTime; }
	void setToken(std::string token) { m_connectionToken = token; }

private:
	unsigned int m_connectionID;
	unsigned int m_connectionTimeout;
	TimeStamp    m_connectionStartTime;
	std::string  m_connectionToken;
};
#endif // !RESTCONNECTION_H