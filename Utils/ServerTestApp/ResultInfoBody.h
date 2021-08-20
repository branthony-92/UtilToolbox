#ifndef RESULTINFO_H
#define RESULTINFO_H

#include "RESTModels.h"

class ResultInfoBody : public JSONInfoBody
{
public:
	ResultInfoBody();

	JSON toJSON() const override;

	void loadJSON(JSON info) override;
	std::string getName() const { return m_name; }
	int getVal() const { return m_val; }

	void setName(std::string name) { m_name = name; }
	void setVal(int val) { m_val = val; }

private:
	std::string m_name;
	int         m_val;
};
typedef std::shared_ptr<ResultInfoBody> ResultInfoPtr;

#endif // !RESULTINFO_H