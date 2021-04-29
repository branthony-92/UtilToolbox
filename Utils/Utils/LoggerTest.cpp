#include "stdafx.h"
#include "LoggerTest.h"
#include "DirectoryTree.h"

void LoggerTest::SetUp()
{
	auto pDir = std::make_shared< CDirectoryTree>(WorkingDirectory::Config, "Test Directory");

	auto pNode = pDir->getDir({"Logger test"}, "Logs", true);

	ASSERT_TRUE(pNode != nullptr);

	auto path = pNode->getPathStr();
	ASSERT_TRUE(!path.empty());

	m_pLogger = std::make_shared<CLogger>(path);
}

void LoggerTest::TearDown()
{
}

TEST_F(LoggerTest, CreateLog)
{
	//m_pLogger->setLogName("TestLog");
	m_pLogger->startLog();
	m_pLogger->logMsg({ Severity::Info, "TestLog 1" });
	m_pLogger->logMsg({ Severity::Info, "TestLog 2" });
	m_pLogger->logMsg({ Severity::Info, "TestLog 3" });
}