#ifndef DIRECTORYMANAGER_H
#define DIRECTORYMANAGER_H

enum class OSType : unsigned int
{
	Windows,
	Linux,
	IOS,
};

enum class WorkingDirectory : unsigned int
{
	Root,
	Config,
	Install,
	CustomAbsolute,
	CustomRelative,
};

enum class DirectoryReturnCode : unsigned int
{
	OK,
	PathAlreadyExists,
	PathDoesNotExist,
	CouldNotCreatePath,
};

class CDirectoryManager
{
public:

	CDirectoryManager(WorkingDirectory workingDir, std::string name);
	virtual ~CDirectoryManager();

	std::string makeDirectory(std::vector<std::string> path, bool recursive = false);

private:
	std::string getWorkingDirStr();

	WorkingDirectory m_workingDirPath;
	std::string m_workingDirName;
};
typedef std::shared_ptr<CDirectoryManager> TDirManagerPtr;
#endif // !DIRECTORYMANAGER_H