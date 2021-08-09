#ifndef RESTCONNECTION_H
#define RESTCONNECTION_H


class ConnectionManager {
public:
	enum class ConnectionStatus {
		Uninitialized,
		Open,
		StaleToken,
		BadToken,
		Closed,

		Invalid = -1,
	};

	enum class TokenType {
		Transient,
		Persistent,
		Invalid = -1,
	};

	typedef std::chrono::system_clock::time_point TimeStamp;
	class Token {
	public:
		std::string  val;
		TimeStamp	 startTime;
		unsigned int timeout;
		TokenType	 type;

		Token() 
			: val("")
			, startTime(std::chrono::system_clock::now())
			, timeout(0)
			, type(TokenType::Invalid)
		{}
		bool compare(const std::string& other) { return val.compare(other) == 0; }
	};

	class Connection;
	typedef std::shared_ptr<ConnectionManager::Connection>  TConnectionPtr;
	typedef std::map<unsigned int, TConnectionPtr>          TConnectionMap;
	typedef std::pair<unsigned int, TConnectionPtr>         TConnectionMapData;

	ConnectionManager();
	virtual ~ConnectionManager();

	// connection management
	TConnectionPtr openNewConnection(Token token);
	TConnectionPtr getConnection(unsigned int id);

	void closeConnection(const unsigned int id);
	ConnectionStatus validateConnection(const unsigned int id, const std::string& token);
	Token refreshConnection(const unsigned int id);

	void removeClosedConnections();
	
private:
	unsigned int generateID();
	std::string generateTokenValue();

private:
	TConnectionMap m_connections;
};
typedef std::shared_ptr<ConnectionManager> TConnectionMgrPtr;

class ConnectionManager::Connection
{

public:
	Connection(unsigned int id = -1)
		: m_connectionID(id)
		, m_connectionToken()
		, m_connectionStatus(ConnectionStatus::Uninitialized)
	{
	}
	Connection(const ConnectionManager::Connection& other)
		: m_connectionID(other.getID())
		, m_connectionToken(other.getToken())
		, m_connectionStatus(other.getStatus())
	{
	}

	Connection& operator=(const ConnectionManager::Connection& other)
	{
		m_connectionID = other.getID();
		m_connectionToken = other.getToken();
		m_connectionStatus = other.getStatus();

		return *this;
	}
	virtual ~Connection() {}

public:
	unsigned int getID()		 const { return m_connectionID; }
	Token  getToken()			 const { return m_connectionToken; }
	ConnectionStatus getStatus() const { return m_connectionStatus; }

	void setID(unsigned int id)				{ m_connectionID = id; }
	void setToken(Token token)				{ m_connectionToken = token; }
	void setStatus(ConnectionStatus state)  { m_connectionStatus = state; }

	bool isTimedOut() const;
	Token& tokenRef() { return m_connectionToken; }

private:
	unsigned int      m_connectionID;
	Token			  m_connectionToken;
	ConnectionStatus  m_connectionStatus;
};
#endif // !RESTCONNECTION_H