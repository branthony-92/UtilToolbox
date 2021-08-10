#ifndef JSONMODELKEYS_H
#define JSONMODELKEYS_H

#include <cpprest/json.h>

using namespace utility;

// JSON key mappings
namespace JSONModelKeys
{
	namespace ResponseKeys {
		const string_t c_headerKey         = U("Response");				 // Object
		const string_t c_endpointXactionID = U("EndpointTransactionID"); // number
		const string_t c_serverXactionID   = U("ServerTransactionID");	 // number

		namespace ErrorInfoKeys {
			const string_t c_headerKey    = U("ErrorInfo");		 // Object
			const string_t c_errorMsgKey  = U("ErrorMessage");   // String
			const string_t c_errorCodeKey = U("ErrorCode");      // number
		}
	}

	namespace ServerInfoKeys {
		const string_t c_headerKey        = U("ServerInfo");	   // Object
		const string_t c_nameKey          = U("Name");             // string
		const string_t c_apiVerKey        = U("APIVersion");       // number
		const string_t c_URLKey		      = U("URL");              // string
		const string_t c_ServerTimeoutKey = U("IdleTimeoutSec");   // string

		namespace ServerURIKeys {
			const string_t c_headerKey = U("URIComponents");  // Object
			const string_t c_schemaKey = U("Schema");         // string
			const string_t c_hostKey   = U("Hostname");       // string
			const string_t c_portKey   = U("Port");           // number
			const string_t c_rootKey   = U("Root");           // string
		}
	}

	namespace ConnectionInfoKeys {
		const string_t c_headerKey          = U("ConnectionInfo");  // Object
		const string_t c_connectionIDKey    = U("ConnectionID");    // number
		const string_t c_connectionTokenKey = U("ConnectionToken"); // string
	}
}

#endif // !JSONMODELKEYS_H