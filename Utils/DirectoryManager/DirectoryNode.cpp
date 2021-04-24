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

CDirectoryTree::TDirPtr CDirectoryTree::CDirectory::getSubDir(unsigned int index)
{
	if (index >= m_subDirs.size())
	{
		return nullptr;
	}
	return m_subDirs[index];
}

void CDirectoryTree::CDirectory::addDir(CDirectoryTree::TDirPtr pDir)
{
	m_subDirs.push_back(pDir);
}

DirectoryReturnCode CDirectoryTree::CDirectory::removeDir(CDirectoryTree::TDirPtr pDir)
{
	auto status = DirectoryReturnCode::PathDoesNotExist;
	for (auto iter = m_subDirs.begin(); iter != m_subDirs.end(); iter++)
	{
		if ((*iter)->getName() == pDir->getName())
		{
			m_subDirs.erase(iter);
			return DirectoryReturnCode::OK;
		}
	}
	return status;
}

CDirectoryTree::TDirPtr CDirectoryTree::CDirectory::findSubDir(std::string name)
{
	for (auto pDir : m_subDirs)
	{
		if (pDir->getName() == name)
		{
			return pDir;
		}
	}
	return nullptr;;
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

