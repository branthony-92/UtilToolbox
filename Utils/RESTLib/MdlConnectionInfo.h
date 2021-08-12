#ifndef MDLCONNECTIONINFO_H
#define MDLCONNECTIONINFO_H

#include "RESTModels.h"
#include "MdlTokenInfo.h"

class ConnectionInfoBody : public JSONInfoBody
{
public:
	ConnectionInfoBody();

	web::json::value toJSON() const override;
	void loadJSON(value info) override;

	std::shared_ptr<TokenInfoBody> getToken()        const { return m_pToken; }
	unsigned int				   getConnectionID() const { return m_id;     }

	void setToken(std::shared_ptr<TokenInfoBody> val) { m_pToken = val; }
	void setConnectionID(unsigned int val)            { m_id = val;     }

private:

	std::shared_ptr<TokenInfoBody> m_pToken;
	unsigned int				   m_id;
};

#endif // !MDLCONNECTIONINFO_H