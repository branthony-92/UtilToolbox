#include "stdafx.h"
#include "LoggerTest.h"
#include "DirectoryTree.h"

void LoggerTest::SetUp()
{
	auto pDir = std::make_shared< CDirectoryTree>(WorkingDirectory::Config, "TestDirectory");

	auto pNode = pDir->getDir("Logs");

	ASSERT_TRUE(pNode != nullptr);

	auto path = pNode->getPathStr();
	ASSERT_TRUE(!path.empty());

	m_pLogger = std::make_shared<CLogger>(path);
	m_pLogger->setLogFileName("TestLog");
}

void LoggerTest::TearDown()
{
}

TEST_F(LoggerTest, CreateLog)
{
	for (auto i = 0; i < 10; i++)
	{
		m_pLogger->logMsg({ Severity::Info, "Test Log" });
	}
}

TEST_F(LoggerTest, HighThroughputLogTest)
{
	m_pLogger->setLogFileName("TestLogHighThroughput");
	for (auto i = 0; i < 1000; i++)
	{
		m_pLogger->logMsg({ Severity::Info, "Test Log" });
	}
}