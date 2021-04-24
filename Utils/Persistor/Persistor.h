#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <nlohmann/json.hpp>
#include <deque>
typedef std::deque<std::string> TKeyPath;

using JSON = nlohmann::json;

class CPersistor
{

public:
	enum class ReturnStatus {
		Invalid = -1,

		OK,
		NoFile,

		Error,
	};

public:
	CPersistor(std::string name = "");
	~CPersistor();

	// write the JSON object to a json file to disk using the set name and path
	ReturnStatus save(); 
	
	// load a json file from disk using the set name and path and store it in the JSON object
	ReturnStatus load(); 

	// recursively locate the appropriate subkey and set it to the provided value
	template <typename T>
	void put(TKeyPath keys, T val); 

	// recursively locate the appropriate subkey and retrieve its value
	template <typename T>
	T get(TKeyPath keys, T defaultVal); 

	// recursively search the entire JSON structure for the existence of a single key returning that keys hit count
	unsigned int searchKeyName(std::string key);
	
	// check if a subkey exist given a path of keys
	bool contains(TKeyPath keys); 

	std::string getJSONString() { return m_persistor.dump(); }

private:
	bool find(JSON& json, TKeyPath& keys);

	template <typename T>
	void putVal(JSON& json, TKeyPath& keys, T& val);

	template <typename T>
	T getVal(JSON& json, TKeyPath& keys, T& def);

public:
	JSON& getPersistenceObject() { return m_persistor; }

	std::string getName() const { return m_persistorName; }
	void setName(std::string name) { m_persistorName = name; }

	std::string getPath() const { return m_persistorPath; }
	void setPath(std::string path) { m_persistorPath = path; }

private:
	JSON m_persistor;

	std::string m_persistorName;
	std::string m_persistorPath;
};

template <typename T>
void CPersistor::putVal(JSON& json, TKeyPath& keys, T& val)
{
	if (keys.empty()) return;
	auto key = keys.front();
	keys.pop_front();

	if (!json.contains(key))
	{
		if (keys.empty())
		{
			json[key] = val;
			return;
		}
		json[key] = nlohmann::json::object();
	}

	if (!json[key].is_object())
	{
		json[key] = val;
		return;
	}
	putVal(json[key], keys, val);
}

template <typename T>
T CPersistor::getVal(JSON& json, TKeyPath& keys, T& def)
{
	if (keys.empty()) return def;

	auto key = keys.front();
	keys.pop_front();

	if (!json.contains(key))
	{
		return def;
	}

	if (!json[key].is_object())
	{
		return json[key];
	}
	return getVal(json[key], keys, def);
}

template <typename T>
void CPersistor::put(TKeyPath keys, T val)
{
	try
	{
		putVal(m_persistor, keys, val);
	}
	catch (std::exception&)
	{

	}
}

template <typename T>
T CPersistor::get(TKeyPath keys, T defaultVal)
{
	try {
		return getVal(m_persistor, keys, defaultVal);
	}
	catch (std::exception&)
	{
		return defaultVal;
	}
}

#endif // !PERSISTOR_H