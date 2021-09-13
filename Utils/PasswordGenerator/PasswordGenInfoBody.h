#ifndef PWGENMODELS_H
#define PWGENMODELS_H

#include "RESTModels.h"

typedef std::vector<char> char_array;

class PasswordInfo : public JSONInfoBody
{
	unsigned int m_charLen;
	unsigned int m_specialCharLen;
	unsigned int m_numLen;
	unsigned int m_passwordLen;
	std::string  m_password;
	std::string  m_specialCharacters;

public:
	PasswordInfo()
		: JSONInfoBody("Password_Info")
		, m_charLen(0)
		, m_specialCharLen(0)
		, m_numLen(0)
		, m_passwordLen(16)
		, m_password("")
		, m_specialCharacters({'_', '!', '&', '$', '~', '%', '\"'})
	{}

	JSON toJSON() const override;
	virtual void  loadJSON(JSON) override;

	unsigned int getCharLen() const { return m_charLen; }
	unsigned int getSpecialCharLen() const { return m_specialCharLen; }
	unsigned int getNumLen() const { return m_numLen; }
	unsigned int getPasswordLen() const { return m_passwordLen; }
	std::string getPassword() const { return m_password; }

	std::string getSpecialCharacters() { return m_specialCharacters; }

	void setCharLen(unsigned int val) { m_charLen = val; }
	void setSpecialCharLen(unsigned int val) { m_specialCharLen = val; }
	void setNumLen(unsigned int val) { m_numLen = val; }
	void setPasswordLen(unsigned int val) { m_passwordLen = val; }
	void setPassword(std::string val) { m_password = val; }
	void setSpecialCharacters(std::string chars) { m_specialCharacters = chars; }

};
typedef std::shared_ptr<PasswordInfo> PasswordInfoPtr;


#endif // !PWGENMODELS_H