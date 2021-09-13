#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include "RESTServerContext.h"
#include "PasswordGenInfoBody.h"

class PasswordGenerator : public RESTServerContext
{
	PasswordInfoPtr m_pInfo;

public: 
	PasswordGenerator();
	virtual ~PasswordGenerator();

	std::string generatePasswordChars(const char_array& arr, unsigned int len = 16);
	unsigned int getRandomIndex(unsigned int maxLen);

	void shuffle(std::string& str);
	std::string generate(const unsigned int shuffleCount = 1);

	PasswordInfoPtr getInfo() const { return m_pInfo;   }
	void setInfo(PasswordInfoPtr pInfo) { m_pInfo = pInfo; }

	std::string findResource(std::string target) override;

};
typedef std::shared_ptr<PasswordGenerator> PasswordGeneratorPtr;

#endif // !PASSWORDGENERATOR_H
