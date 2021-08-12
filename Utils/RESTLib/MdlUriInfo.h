#ifndef MDLURIINFO_H
#define MDLURIINFO_H

#include "RESTModels.h"

class URIInfoBody : public JSONInfoBody
{
public:
	URIInfoBody();

	virtual value toJSON() const override;
	virtual void loadJSON(value info) override;

	std::string  getSchema() { return m_schema; }
	std::string  getHost()   { return m_host;   } 
	unsigned int getPort()   { return m_port;   } 
	std::string  getRoot()   { return m_root;   } 

	void setSchema(std::string val)  { m_schema = val; }
	void setHost(std::string val)    { m_host = val;   }
	void setPort(unsigned int val)   { m_port = val;   }
	void setRoot(std::string val)    { m_root = val;   }

private:
	std::string  m_schema;
	std::string  m_host;
	unsigned int m_port;
	std::string  m_root;
};

#endif // !MDLURIINFO_H