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
	const std::map<JSONInfoBody::BodyType, std::string> c_bodyIDToString =
	{
		{ JSONInfoBody::BodyType::Body_URIInfo,        "URI_Info"         },
		{ JSONInfoBody::BodyType::Body_ErrorInfo,	   "Error_Info"       },
		{ JSONInfoBody::BodyType::Body_ReponseInfo,	   "Reponse_Info"     },
		{ JSONInfoBody::BodyType::Body_ServerInfo,	   "Server_Info"      },
		{ JSONInfoBody::BodyType::Body_ConnectionInfo, "Connection_Info" },
		{ JSONInfoBody::BodyType::Body_TokenInfo,	   "Token_Info"      }
	};

	const std::map<std::string, JSONInfoBody::BodyType> c_bodyNameToID =
	{
		{ "URI_Info",        JSONInfoBody::BodyType::Body_URIInfo         },
		{ "Error_Info",      JSONInfoBody::BodyType::Body_ErrorInfo       },
		{ "Reponse_Info",    JSONInfoBody::BodyType::Body_ReponseInfo	  },
		{ "Server_Info",     JSONInfoBody::BodyType::Body_ServerInfo	  },
		{ "Connection_Info", JSONInfoBody::BodyType::Body_ConnectionInfo  },
		{ "Token_Info",      JSONInfoBody::BodyType::Body_TokenInfo		  }
	};
}

JSONInfoBody::JSONInfoBody(BodyType ID)
	: c_name(c_bodyIDToString.at(ID))
{
}

JSONInfoBody::JSONInfoBody(std::string name)
	: c_name(name)
{
}

std::shared_ptr<JSONInfoBody> JSONInfoBody::createBody(std::string name)
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
