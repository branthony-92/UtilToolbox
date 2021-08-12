#ifndef MDLRESPONSEINFO_H
#define MDLRESPONSEINFO_H

#include "RESTModels.h"

class ResponseInfoBody : public JSONInfoBody
{
public:
	ResponseInfoBody();
	virtual ~ResponseInfoBody() {}

	virtual value toJSON() const override;
	virtual void loadJSON(value info) override;

	void addBody(std::shared_ptr<JSONInfoBody> pBody);

	std::shared_ptr<JSONInfoBody> findBody(string_t name) const;

	unsigned int getTransactionID() const { return m_transactionID; }
	void setTransactionID(unsigned int id) { m_transactionID = id; }

private:
	unsigned int m_transactionID;
	BodyMap      m_bodies;
};

#endif // !MDLRESPONSEINFO_H