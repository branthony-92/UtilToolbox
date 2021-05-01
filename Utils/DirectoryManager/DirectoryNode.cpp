#include "stdafx.h"
#include "DirectoryTree.h"

CDirectoryTree::CDirectory::CDirectory(TDirManagerPtr pDrMgr, TPath path, std::string dir)
	: m_subDirs()
	, m_dirName(dir)
	, m_dirPath("")
	, m_parentDirs(path)
	, m_pDirManager(pDrMgr)
{
}

CDirectoryTree::CDirectory::~CDirectory()
{
}

CDirectoryTree::TDirPtr CDirectoryTree::CDirectory::getSubDir(std::string dirName)
{
	if (m_subDirs.find(dirName) == m_subDirs.end()) 
	{
		return nullptr;
	}
	return m_subDirs[dirName];
}

void CDirectoryTree::CDirectory::addDir(CDirectoryTree::TDirPtr pDir)
{
	if (m_subDirs.find(pDir->getName()) != m_subDirs.end()) return;
	m_subDirs.insert(TDirListData(pDir->getName(), pDir));
}

DirectoryReturnCode CDirectoryTree::CDirectory::removeDir(CDirectoryTree::TDirPtr pDir)
{
	auto status = DirectoryReturnCode::PathDoesNotExist;
	auto removed = m_subDirs.erase(pDir->getName());
	if (removed > 0) status = DirectoryReturnCode::OK;
	return status;
}

CDirectoryTree::TDirPtr CDirectoryTree::CDirectory::findSubDir(std::string name)
{
	if (m_subDirs.find(name) != m_subDirs.end())
	{
		return m_subDirs[name];
	}
	return nullptr;
}

DirectoryReturnCode CDirectoryTree::CDirectory::createPath(bool createRecursive)
{
	auto path = m_parentDirs;
	path.push_back(m_dirName);
	m_dirPath = m_pDirManager->makeDirectory(path, createRecursive);
	if (m_dirPath.empty()) return DirectoryReturnCode::CouldNotCreatePath;
	return DirectoryReturnCode::OK;
}

std::string CDirectoryTree::CDirectory::getPathStr() const
{
	return m_dirPath;
}

