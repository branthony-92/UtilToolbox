#ifndef PERSISTORTEST_H
#define PERSISTORTEST_H

#include "Persistor.h"
#include "DirectoryTree.h"
#include "gtest/gtest.h"

class PersistorTest : public ::testing::Test
{
	std::shared_ptr<CPersistor>     m_pPers;
	std::shared_ptr<CDirectoryTree> m_pDir;
	std::string						m_path;

protected:
	void SetUp()
	{
		m_pDir = std::make_shared< CDirectoryTree>(WorkingDirectory::Config, "Test Directory");
		auto pNode = m_pDir->getDir({ }, "Json Test", true);

		if (!pNode)
		{
			throw std::runtime_error("Folder could not found/created");
		}
		m_path = pNode->getPathStr();
		m_pPers = std::make_shared<CPersistor>();
		m_pPers->setPath(m_path);
	}

	void TearDown()
	{
		m_pPers = nullptr;
		m_pDir = nullptr;
	}

	std::shared_ptr<CPersistor> pers() { return m_pPers; }
};

#endif // !