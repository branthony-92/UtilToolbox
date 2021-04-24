#include "stdafx.h"
#include "DirectoryManager.h"
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

#if _WIN32
	constexpr OSType c_osID = OSType::Windows;
	constexpr char c_delim = '\\';
	const std::string c_rootDir = std::string(getenv("HOMEDRIVE"));
	const std::string c_installDir = std::string(getenv("PROGRAMFILES"));
	const std::string c_configDir = std::string(getenv("APPDATA"));
#elif defined(__APPLE__) || defined(__MACH__)
	constexpr char c_delim = '/';
	constexpr CDirectoryManager::OSType c_osID = CDirectoryManager::OSType::IOS;
	const std::string c_rootDir = std::string(getenv("ROOT"));
	const std::string c_installDir = std::string(getenv("PROGRAMFILES"));
	const std::string c_configDir = std::string(getenv("HOME"));
#elif defined(__LINUX__)
	constexpr char c_delim = '/';
	constexpr CDirectoryManager::OSType c_osID = CDirectoryManager::OSType::Linux;
	const std::string c_rootDir = std::string(getenv("ROOT"));
	const std::string c_installDir = std::string(getenv("PROGRAMFILES"));
	const std::string c_configDir = std::string(getenv("HOME"));
#endif 

	const std::string c_relativeCurrentDir = ".";
	const std::string c_relativeParentDir = "..";

CDirectoryManager::CDirectoryManager(WorkingDirectory workingDir, std::string name)
	: m_workingDirPath(workingDir)
	, m_workingDirName(name)
{
}

CDirectoryManager::~CDirectoryManager()
{
}

std::string CDirectoryManager::getWorkingDirStr()
{
	std::string path = "";
	switch (m_workingDirPath)
	{
	case WorkingDirectory::Root:
		path = c_rootDir;
		path += c_delim;
		path += m_workingDirName;
		path += c_delim;
		break;
	case WorkingDirectory::Config:
		path = c_configDir;
		path += c_delim;
		path += m_workingDirName;
		path += c_delim;
		break;
	case WorkingDirectory::Install:
		path = c_installDir;
		path += c_delim;
		path += m_workingDirName;
		path += c_delim;
		break;
	case WorkingDirectory::CustomAbsolute:
		break;
	case WorkingDirectory::CustomRelative:
		path += c_relativeCurrentDir;
		path += c_delim;
		break;
	default:
		break;
	}

	return path;
}

std::string CDirectoryManager::makeDirectory(std::vector<std::string> path, bool recursive)
{
	std::string result = "";
	result += getWorkingDirStr();

	std::string currentDir = path.back();
	for (auto i = 0u; i < path.size(); i++)
	{
		result += path[i];
		if (i != path.size() - 1) result += c_delim;
	}

	if (currentDir != c_relativeCurrentDir && currentDir != c_relativeParentDir)
	{
		if (recursive) fs::create_directories(result);
		else		   fs::create_directory(result);
	}

	result += c_delim;
	return result;
}

