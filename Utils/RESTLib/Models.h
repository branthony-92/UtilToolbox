#ifndef MODELS_H
#define MODELS_H

#include "JSONModelKeys.h" 

#include <set>

#include "RESTServerException.h"
#include "RESTConnection.h"


struct ServerURI {
	std::string  schema;
	std::string  host;
	unsigned int port;
	std::string  root;
};

class ResponseBody {
	RESTServerException m_exception;
	unsigned int m_transactionID;

public:
	ResponseBody(unsigned int id, RESTServerException ex = RESTServerException(""))
		: m_transactionID(id)
		, m_exception(ex)
	{}

	web::json::value toJson() {
		using namespace JSONModelKeys::ResponseKeys;

		web::json::value response = web::json::value::object();
		response[ErrorInfoKeys::c_headerKey] = m_exception.toJson();
		response[c_endpointXactionID]    = web::json::value::number(m_transactionID);
		return response;
	}
};

class ServerInfo {
public:
	enum class ServerStatus {
		Uninitialized,
		Listening,
		Error,
	};

	ServerInfo()
		: serverName("")
		, APIVersion(1.0)
		, URLString("")
		, URI()
		, serverIdleTimoutSec(UINT_MAX) // basically forever
		, serverState(ServerStatus::Uninitialized)
		, endpoints()
	{}

	std::string     serverName;
	double		    APIVersion;
	std::string     URLString;
	ServerURI       URI;
	unsigned int    serverIdleTimoutSec;
	ServerStatus	serverState;
	std::set<std::string> endpoints;

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

		using namespace JSONModelKeys::ServerInfoKeys;

		web::json::value info = web::json::value::object();
		web::json::value uriInfo = web::json::value::object();

		auto name = utility::conversions::to_string_t(serverName);
		auto url = utility::conversions::to_string_t(URLString);

		info[c_nameKey]          = web::json::value::string(name);
		info[c_apiVerKey]        = web::json::value::number(APIVersion);
		info[c_URLKey]           = web::json::value::string(url);
		info[c_ServerTimeoutKey] = web::json::value::number(serverIdleTimoutSec);

		web::json::value ar = web::json::value::array(endpoints.size());

		auto index = 0u;
		for (auto& e : endpoints)
		{
			auto ep = utility::conversions::to_string_t(e);
			ar[index++] = web::json::value::string(ep);
		}
		info[c_ServerEndpointsKey] = ar;

		auto schema = utility::conversions::to_string_t(URI.schema);
		auto host = utility::conversions::to_string_t(URI.host);
		auto root = utility::conversions::to_string_t(URI.root);

		uriInfo[ServerURIKeys::c_schemaKey] = web::json::value::string(schema);
		uriInfo[ServerURIKeys::c_portKey]   = web::json::value::number(URI.port);
		uriInfo[ServerURIKeys::c_hostKey]   = web::json::value::string(host);
		uriInfo[ServerURIKeys::c_rootKey]   = web::json::value::string(root);

		info[ServerURIKeys::c_headerKey] = uriInfo;
		return info;
	}
};

class ConnectionInfo {
public:
	ConnectionInfo()
		: token()
		, id(0)
	{}

	ConnectionManager::Token token;
	unsigned int             id;

	web::json::value toJSON() const {
		using namespace JSONModelKeys::ConnectionInfoKeys;
		web::json::value info = web::json::value::object();

		auto tokenStr = utility::conversions::to_string_t(token.val);

		info[c_connectionTokenKey] = web::json::value::string(tokenStr);
		info[c_connectionIDKey]    = web::json::value::number(id);
		return info;
	}
};

#endif // !MODELS_H