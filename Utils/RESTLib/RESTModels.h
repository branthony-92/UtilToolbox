#ifndef RESTMODELS_H
#define RESTMODELS_H

#include <set>
#include <map>
#include <cpprest/json.h>

using namespace utility;
using namespace utility::conversions;
using namespace web::json;

class JSONInfoBody {
public:

	enum class BodyType
	{
		FirstBody = 0,

		Body_URIInfo = FirstBody,
		Body_ErrorInfo,
		Body_ReponseInfo,
		Body_ServerInfo,
		Body_ConnectionInfo,
		Body_TokenInfo,
		
		NumBodies,

		Body_Invalid = -1,
	};

	JSONInfoBody(BodyType ID = BodyType::Body_Invalid);
	JSONInfoBody(string_t name);
	virtual ~JSONInfoBody() {}

	virtual value toJSON() const = 0;
	virtual void  loadJSON(value) = 0;
	
	const string_t c_name;

	static std::shared_ptr<JSONInfoBody> createBody(BodyType ID);
	static std::shared_ptr<JSONInfoBody> createBody(string_t name);

};
typedef std::map<string_t,std::shared_ptr<JSONInfoBody>> BodyMap;

#endif // !RESTMODELS_H