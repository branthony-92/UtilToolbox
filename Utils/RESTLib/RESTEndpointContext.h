#ifndef RESTENDPOINTCONTEXT_H
#define RESTENDPOINTCONTEXT_H

#include <cpprest/json.h>
#include <set>

// JSON key mappings
namespace JSONModelKeys
{
	namespace General
	{
		const utility::string_t c_xactionID = U("TransactionID"); // number
	}
	namespace ErrorBody
	{
		const utility::string_t c_errorKey = U("Error");         // String
		const utility::string_t c_errorCodeKey = U("ErrorCode"); // number
	}

	const utility::string_t c_serverInfoKey = U("ServerInfo"); // Object
	namespace ServerInfo {

		const utility::string_t c_nameKey       = U("Name");        // string
		const utility::string_t c_apiVerKey     = U("APIVersion");  // number
		const utility::string_t c_URLKey        = U("URL");         // string

		const utility::string_t c_serverURIKey = U("ServerURIParts"); // Object
		namespace ServerURI {
			const utility::string_t c_schemaKey = U("Schema");   // string
			const utility::string_t c_hostKey   = U("Hostname"); // string
			const utility::string_t c_portKey   = U("Port");     // number
			const utility::string_t c_rootKey   = U("Root");     // string
		}
	}
}

struct ServerURI {
	std::string  schema;
	std::string  host;
	unsigned int port;
	std::string  root;
};

class ServerInfo {
public:
	enum class ServerStatus {
		Uninitialized,
		Listening,
		Error,
	} serverState;

	std::string    serverName;
	double		   APIVersion;
	std::string    URLString;
	ServerURI      URI;

	std::string stringFromState(ServerStatus state)
	{
		std::string name = "";

		switch (state)
		{
		case ServerStatus::Uninitialized: name = "Uninitialized"; break;
		case ServerStatus::Listening:     name = "Listening"; break;
		case ServerStatus::Error:         name = "Listening"; break;
		default:                          name = "Unknown "; break;
		}
		return name;
	}

	web::json::value toJSON() const {
		web::json::value info = web::json::value::object();
		web::json::value uriInfo = web::json::value::object();

		auto name   = utility::conversions::to_string_t(serverName);
		auto url    = utility::conversions::to_string_t(URLString);

		info[JSONModelKeys::ServerInfo::c_nameKey]      = web::json::value::string(name);
		info[JSONModelKeys::ServerInfo::c_apiVerKey]    = web::json::value::number(APIVersion);
		info[JSONModelKeys::ServerInfo::c_URLKey]       = web::json::value::string(url);

		auto schema = utility::conversions::to_string_t(URI.schema);
		auto host   = utility::conversions::to_string_t(URI.host);
		auto root   = utility::conversions::to_string_t(URI.root);

		uriInfo[JSONModelKeys::ServerInfo::ServerURI::c_schemaKey] = web::json::value::string(schema);
		uriInfo[JSONModelKeys::ServerInfo::ServerURI::c_portKey]   = web::json::value::number(URI.port);
		uriInfo[JSONModelKeys::ServerInfo::ServerURI::c_hostKey]   = web::json::value::string(host);
		uriInfo[JSONModelKeys::ServerInfo::ServerURI::c_rootKey]   = web::json::value::string(root);
		
		info[JSONModelKeys::ServerInfo::c_serverURIKey] = uriInfo;
		return info;
	}
};

class RESTEndpointContext
{
protected:

	std::string m_name;

	ServerInfo m_serverInfo;

	std::atomic_bool m_stopFlag;
	std::atomic_bool m_resetFlag;

	std::set<std::string> m_endpoints;

public:
	RESTEndpointContext(std::string name) 
		: m_name(name)
		, m_stopFlag(false)
		, m_resetFlag(false)
		, m_serverInfo()
	{}

	ServerInfo& serverInfo() { return m_serverInfo; }

	virtual ~RESTEndpointContext() {}

	bool addEndpoint(std::string name);

	std::string getName() const { return m_name; }
	std::set<std::string> getEndpoints() const { return m_endpoints; }

	void stop()  { m_stopFlag = true; }
	void reset() { m_resetFlag = true; }

	bool stopRequested();
	bool resetRequested();
};
typedef std::shared_ptr<RESTEndpointContext> TRESTCtxPtr;

#endif // !RESTENDPOINTCONTEXT_H
