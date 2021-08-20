#ifndef RESTMODELS_H
#define RESTMODELS_H
#include "nlohmann/json.hpp"
#include <set>
#include <map>

using JSON = nlohmann::json;


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
	JSONInfoBody(std::string name);
	virtual ~JSONInfoBody() {}

	virtual JSON toJSON() const = 0;
	virtual void  loadJSON(JSON) = 0;
	
	const std::string c_name;

	static std::shared_ptr<JSONInfoBody> createBody(BodyType ID);
	static std::shared_ptr<JSONInfoBody> createBody(std::string name);

};
typedef std::map<std::string,std::shared_ptr<JSONInfoBody>> BodyMap;

#endif // !RESTMODELS_H