#include "stdafx.h"
#include "RESTModels.h"

#include "MdlConnectionInfo.h"
#include "MdlErrorInfo.h"
#include "MdlResponseInfo.h"
#include "MdlServerInfo.h"
#include "MdlTokenInfo.h"
#include "MdlUriInfo.h"

namespace
{
	const std::map<JSONInfoBody::BodyType, string_t> c_bodyIDToString =
	{
		{ JSONInfoBody::BodyType::Body_URIInfo,        U("URI_Info")         },
		{ JSONInfoBody::BodyType::Body_ErrorInfo,	   U("Error_Info")       },
		{ JSONInfoBody::BodyType::Body_ReponseInfo,	   U("Reponse_Info")     },
		{ JSONInfoBody::BodyType::Body_ServerInfo,	   U("Server_Info")      },
		{ JSONInfoBody::BodyType::Body_ConnectionInfo, U("BConnection_Info") },
		{ JSONInfoBody::BodyType::Body_TokenInfo,	   U("BToken_Info")      }
	};

	const std::map<string_t, JSONInfoBody::BodyType> c_bodyNameToID =
	{
		{ U("URI_Info"),        JSONInfoBody::BodyType::Body_URIInfo         },
		{ U("Error_Info"),      JSONInfoBody::BodyType::Body_ErrorInfo       },
		{ U("Reponse_Info"),    JSONInfoBody::BodyType::Body_ReponseInfo	 },
		{ U("Server_Info"),     JSONInfoBody::BodyType::Body_ServerInfo	     },
		{ U("Connection_Info"), JSONInfoBody::BodyType::Body_ConnectionInfo  },
		{ U("Token_Info"),      JSONInfoBody::BodyType::Body_TokenInfo		 }
	};
}

JSONInfoBody::JSONInfoBody(BodyType ID)
	: c_name(c_bodyIDToString.at(ID))
{
}

JSONInfoBody::JSONInfoBody(string_t name)
	: c_name(name)
{
}

std::shared_ptr<JSONInfoBody> JSONInfoBody::createBody(string_t name)
{
	// look for the bodies we may want and add it to the response body list
	auto bodyNameID = c_bodyNameToID.find(name);
	if (bodyNameID != c_bodyNameToID.end())
	{
		auto ID = bodyNameID->second;
		return createBody(ID);
	}
	return nullptr;
}

std::shared_ptr<JSONInfoBody> JSONInfoBody::createBody(BodyType ID)
{
	std::shared_ptr<JSONInfoBody> pBody = nullptr;
	switch (ID)
	{
	case JSONInfoBody::BodyType::Body_URIInfo:
		pBody = std::make_shared<URIInfoBody>();
		break;
	case JSONInfoBody::BodyType::Body_ErrorInfo:
		pBody = std::make_shared<ErrorInfoBody>();
		break;
	case JSONInfoBody::BodyType::Body_ReponseInfo:
		pBody = std::make_shared<ResponseInfoBody>();
		break;
	case JSONInfoBody::BodyType::Body_ServerInfo:
		pBody = std::make_shared<ServerInfoBody>();
		break;
	case JSONInfoBody::BodyType::Body_ConnectionInfo:
		pBody = std::make_shared<ConnectionInfoBody>();
		break;
	case JSONInfoBody::BodyType::Body_TokenInfo:
		pBody = std::make_shared<TokenInfoBody>();
		break;
	default:
		break;
	}
	return pBody;
}
