#include "stdafx.h"
#include "PasswordGenInfoBody.h"

JSON PasswordInfo::toJSON() const
{
    JSON info = JSON::object();

    info["num_characters"] = m_charLen;
    info["num_special_characters"] = m_specialCharLen;
    info["num_numbers"] = m_numLen;
    info["password_len"] = m_passwordLen;
    info["password"] = m_password;
    info["valid_special_chars"] = m_specialCharacters;

    return info;
}

void PasswordInfo::loadJSON(JSON info)
{
    m_charLen = info["num_characters"].get<unsigned int>();
    m_specialCharLen = info["num_special_characters"].get<unsigned int>();
    m_numLen = info["num_numbers"].get<unsigned int>();
    m_passwordLen = info["password_len"].get<unsigned int>();
    m_password = info["password"].get<std::string>();
    m_specialCharacters = info["valid_special_chars"].get<std::string>();
}
