#ifndef MDLSERVERINFO_H
#define MDLSERVERINFO_H

#include "RESTModels.h"
#include "MdlUriInfo.h"

class ServerInfoBody : public JSONInfoBody
{
public:
	enum class ServerStatus {
		Uninitialized,
		Listening,
		Error,
	};

	ServerInfoBody();

	std::string              setServerName()    const { return m_serverName;          }
	double		             getAPIVer()        const { return m_APIVersion;          }
	std::string              getURLString()     const { return m_URLString;           }
	std::shared_ptr<URIInfoBody> getURI()		    const { return m_pURI;				  }
	unsigned int             getIdleTimeout()   const { return m_serverIdleTimoutSec; }
	ServerStatus	         getState()			const { return m_serverState;         }
	std::set<std::string>    getEndpointNames() const { return m_endpointNames;       }

	void setServerName(std::string val)               { m_serverName = val;          }
	void setAPIVer(double val)                        { m_APIVersion = val;          }
	void setURLString(std::string val)                { m_URLString = val;           }
	void setURI(std::shared_ptr<URIInfoBody> val)         { m_pURI = val;			     }
	void setIdleTimeout(unsigned int val)             { m_serverIdleTimoutSec = val; }
	void setState(ServerStatus val)                   { m_serverState = val;         }
	void setEndpointNames(std::set<std::string> val)  { m_endpointNames = val;       }

private:
	std::string              m_serverName;
	double		             m_APIVersion;
	std::string              m_URLString;
	std::shared_ptr<URIInfoBody> m_pURI;
	unsigned int             m_serverIdleTimoutSec;
	ServerStatus	         m_serverState;
	std::set<std::string>    m_endpointNames;

	std::string stringFromState(ServerStatus state);
	web::json::value toJSON() const override;

	void loadJSON(value info) override;
};

#endif // !MDLSERVERINFO_H