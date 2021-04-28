#include "stdafx.h"
#include "LoggerTest.h"
#include "DirectoryTree.h"

void LoggerTest::SetUp()
{
	auto pDir = std::make_shared< CDirectoryTree>(WorkingDirectory::CustomAbsolute, "TestDirectory");
	std::vector<std::string> p = { "C:", "Users", "xs_br", "source", "repos", "branthony - 92", "UtilToolbox", "Utils", "Utils", "Logger_Test" };

	auto pNode = pDir->getDir( p, "Logs", true);

	ASSERT_TRUE(pNode != nullptr);

	auto path = pNode->getPathStr();
	ASSERT_TRUE(!path.empty());

	m_pLogger = std::make_shared<CLogger>(path);
}

void LoggerTest::TearDown()
{
	m_pLogger->stopLog();
}

TEST_F(LoggerTest, CreateLog)
{
	m_pLogger->startLog();
	m_pLogger->logMsg({ Severity::Info, "TestLog 1" });
	m_pLogger->logMsg({ Severity::Info, "TestLog 2" });
	m_pLogger->logMsg({ Severity::Info, "TestLog 3" });
}