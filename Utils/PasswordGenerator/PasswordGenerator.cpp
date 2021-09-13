
#include "stdafx.h"

#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm>  //for std::generate_n
#include <fstream>
#include "PasswordGenerator.h"


const char_array c_tokenChars = {
	'0','1','2','3','4','5','6','7','8','9',
	'A','B','C','D','E','F','G','H','I','J','K','L','M',
	'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'a','b','c','d','e','f','g','h','i','j','k','l','m',
	'n','o','p','q','r','s','t','u','v','w','x','y','z',
	'_', '!'
};


const char_array c_tokenNums = {
	'0','1','2','3','4','5','6','7','8','9'
};

const std::vector<unsigned int> c_passwordLengthOptions = { 16, 32, 64, 128, 256, 512 };
const auto c_dedaultPWLength = c_passwordLengthOptions[0];


PasswordGenerator::PasswordGenerator()
	: RESTServerContext("Password_Generator")
	, m_pInfo(std::make_shared<PasswordInfo>())
{
}

PasswordGenerator::~PasswordGenerator()
{
}

std::string PasswordGenerator::generatePasswordChars(const char_array& arr, unsigned int len)
{
	//1) create a non-deterministic random number generator      
	std::default_random_engine rng(std::random_device{}());

	//2) create a random number "shaper" that will give
	//   us uniformly distributed indices into the character set
	std::uniform_int_distribution<> dist(0u, arr.size() - 1u);

	//3) create a function that ties them together, to get:
	//   a non-deterministic uniform distribution from the 
	//   character set of your choice.
	auto randchar = [&]() {return arr[dist(rng)]; };

	//4) set the length of the string you want and profit!        
	std::string str(len, 0);

	std::generate_n(str.begin(), len, randchar);
	return str;
}

unsigned int PasswordGenerator::getRandomIndex(unsigned int maxLen)
{
	//1) create a non-deterministic random number generator      
	std::default_random_engine rng(std::random_device{}());

	//2) create a random number "shaper" that will give
	//   us uniformly distributed indices into the character set
	std::uniform_int_distribution<> dist(0u, maxLen - 1u);
	return dist(rng);
}

void PasswordGenerator::shuffle(std::string& str)
{
	for (auto i = 0; i < str.length(); i++)
	{
		auto index1 = getRandomIndex(str.length());
		auto index2 = getRandomIndex(str.length());

		auto temp = str[index1];
		str[index1] = str[index2];
		str[index2] = temp;
	}
}

std::string PasswordGenerator::generate(const unsigned int shuffleCount)
{
	std::string password = "";

	auto charLen = m_pInfo->getCharLen();
	auto specialCharLen = m_pInfo->getSpecialCharLen();
	auto numLen = m_pInfo->getNumLen();
	char_array specialCharsList;

	for (auto c : m_pInfo->getSpecialCharacters())
	{
		specialCharsList.push_back(c);
	}

	password.append(generatePasswordChars(c_tokenChars, charLen));
	password.append(generatePasswordChars(specialCharsList, specialCharLen));
	password.append(generatePasswordChars(c_tokenNums, numLen));

	// shuffle a few times
	for (auto i = 0u; i < shuffleCount; i++) shuffle(password);
	return password;
}

std::string PasswordGenerator::findResource(std::string target)
{
	if (target == "/")          return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\index.html";
	else if (target == "/test") return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\Test\\test.html";
	else if (target == "/img")  return "C:\\Users\\xs_br\\source\\repos\\branthony-92\\UtilToolbox\\Utils\\HTML\\Test\\img.jpg";
	return "";
}
