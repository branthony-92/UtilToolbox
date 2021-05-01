#include "stdafx.h"
#include "DirectoryTree.h"


CDirectoryTree::CDirectoryTree(WorkingDirectory workingDir, std::string name)
	: m_pRoot(nullptr)
	, m_nodeCount(0)
{
	m_pDirMgr = std::make_shared<CDirectoryManager>(workingDir, name);
	m_pRoot = std::make_shared<CDirectory>(m_pDirMgr, TPath(), name);
	m_nodeCount++;
}

CDirectoryTree::TDirPtr CDirectoryTree::getDir(TPath path, std::string dirName, bool createParents)
{
	auto pNode = m_pRoot;
	TPath tempPath;
	for (auto p : path)
	{
		auto pTempNode = pNode->findSubDir(p);
		if (!pTempNode)
		{
			if (!createParents) return nullptr;

			TDirPtr pNewDir = std::make_shared<CDirectory>(m_pDirMgr, tempPath, p);
			pNewDir->createPath(true);
			m_nodeCount++;
			pNode->addDir(pNewDir);
			pTempNode = pNewDir;
		}
		pNode = pTempNode;
		tempPath.push_back(p);
	}
	TDirPtr pNewDir = pNode->findSubDir(dirName);

	if (!pNewDir)
	{
		pNewDir = std::make_shared<CDirectory>(m_pDirMgr, path, dirName);
		pNewDir->createPath(true);
		pNode->addDir(pNewDir);
		m_nodeCount++;
	}
	return pNewDir;
}

CDirectoryTree::TDirPtr CDirectoryTree::getDir(std::string dirName)
{
	TDirPtr pNewDir = m_pRoot->findSubDir(dirName);

	if (!pNewDir)
	{
		pNewDir = std::make_shared<CDirectory>(m_pDirMgr, TPath(), dirName);
		pNewDir->createPath(true);
		m_pRoot->addDir(pNewDir);
		m_nodeCount++;
	}
	return pNewDir;
}


std::vector<CDirectoryTree::TDirPtr> CDirectoryTree::search(std::string val)
{
	std::vector<TDirPtr> dirs;

	std::deque<TDirPtr> queue;
	queue.push_back(m_pRoot);

	while (!queue.empty())
	{
		auto pNext = queue.front();
		queue.pop_front();

		for (auto pDir : pNext->getSubDirs())
		{
			if (pDir.first == val)
			{
				dirs.push_back(pDir.second);
			}
			queue.push_back(pDir.second);
		}
	}
	return dirs;
}

