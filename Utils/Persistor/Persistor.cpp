#include "stdafx.h"
#include "Persistor.h"

#include <fstream>
#include <iomanip>      // std::setw


CPersistor::CPersistor(std::string name)
	: m_persistor()
	, m_persistorName(name)
	, m_persistorPath("")
{
}

CPersistor::~CPersistor()
{
}

CPersistor::ReturnStatus CPersistor::save()
{
	std::ostringstream oss;

	oss << m_persistorPath << m_persistorName << ".json";

	try
	{
		std::ofstream file(oss.str());
		if (!file.is_open()) return ReturnStatus::Error;

		file << std::setw(4) << m_persistor << std::endl;
		file.close();
		return ReturnStatus::OK;
	}
	catch (std::exception&)
	{
		return ReturnStatus::Error;
	}
}

CPersistor::ReturnStatus CPersistor::load()
{
	std::ostringstream oss;

	oss << m_persistorPath << m_persistorName << ".json";

	try
	{
		std::ifstream file(oss.str());
		if (!file.is_open()) return ReturnStatus::NoFile;
		
		file >> m_persistor;
		file.close();
		return ReturnStatus::OK;
		
	}
	catch (std::exception&)
	{
		return ReturnStatus::Error;
	}
}

bool CPersistor::contains(TKeyPath path)
{
	auto json = m_persistor;
	for (auto key : path)
	{
		if (!json.contains(key)) return false;
		json = json[key];
	}
	return true;
}

bool CPersistor::find(JSON& json, TKeyPath& path)
{
	if (path.empty()) return false;

	auto key = path.front();
	path.pop_front();

	if (path.empty())
	{
		return json.contains(key);
	}

	return find(json[key], path);
}

unsigned int CPersistor::searchKeyName(std::string key)
{
	std::deque<JSON> queue;
	queue.push_back(m_persistor);
	unsigned int hitCount = 0;

	while (!queue.empty())
	{
		auto json = queue.front();
		queue.pop_front();

		if (json.contains(key))
		{
			hitCount++;
		}

		for (auto element : json)
		{
			if (element.is_object())
			{
				queue.push_back(element);
			}
		}
	}
	return hitCount;
}