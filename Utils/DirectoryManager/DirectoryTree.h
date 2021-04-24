#ifndef DIRECTORYTREE_H
#define DIRECTORYTREE_H

#include "DirectoryManager.h"

class CDirectoryTree
{
	class CDirectory;
	typedef std::shared_ptr<CDirectory> TDirPtr;
	typedef std::vector<TDirPtr> TDirList;
	typedef std::vector<std::string> TPath;

	TDirPtr m_pRoot;
	TDirManagerPtr m_pDirMgr;
	unsigned int m_nodeCount;

public:

	CDirectoryTree(WorkingDirectory workingDir, std::string name);

	unsigned int getNodeCount() const { return m_nodeCount; }

	TDirPtr getDir(TPath path, std::string dirName, bool createParents = false);
	TDirPtr getDir(std::string dirName);

	TDirList search(std::string val);

};
typedef std::shared_ptr<CDirectoryTree> TDirTreePtr;


class CDirectoryTree::CDirectory
{
	CDirectoryTree::TDirList m_subDirs;

	std::string	    m_dirName;
	std::string		m_dirPath;

	TPath		    m_parentDirs;
	TDirManagerPtr	m_pDirManager;

public:
	CDirectory(TDirManagerPtr pDrMgr, TPath path, std::string dir);
	virtual ~CDirectory();

	size_t subDirCount()    const { return m_subDirs.size(); }
	std::string getName()	const { return m_dirName; }

	CDirectoryTree::TDirList& getSubDirs() { return m_subDirs; }

	CDirectoryTree::TDirPtr getSubDir(unsigned int index);
	void addDir(CDirectoryTree::TDirPtr pDir);
	DirectoryReturnCode removeDir(CDirectoryTree::TDirPtr pDir);
	CDirectoryTree::TDirPtr findSubDir(std::string name);
	DirectoryReturnCode createPath(bool createRecursive = false);
	std::string getPathStr() const;
};
#endif // !DIRECTORYTREE_H