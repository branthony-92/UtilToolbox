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
	m_pLogger->enableLog(true);
}

void LoggerTest::TearDown()
{
}

TEST_F(LoggerTest, CreateLog)
{
	m_pLogger->setWriteMode(WriteMode::TRUNCATE);
	m_pLogger->logMsg({ Severity::Info, "Test Log Start" });

	m_pLogger->setWriteMode(WriteMode::APPEND);
	for (auto i = 0; i < 10; i++)
	{
		std::stringstream ss;
		ss << "Test Log " << i + 1;
		m_pLogger->logMsg({ Severity::Info, ss.str() });
	}
}

TEST_F(LoggerTest, HighThroughputLogTest)
{
	m_pLogger->setLogFileName("TestLogHighThroughput");
	m_pLogger->setWriteMode(WriteMode::TRUNCATE);
	m_pLogger->logMsg({ Severity::Info, "Test Log Start" });

	m_pLogger->setWriteMode(WriteMode::APPEND);
	for (auto i = 0; i < 1000; i++)
	{
		std::stringstream ss;
		ss << "Test Log " << i + 1;
		m_pLogger->logMsg({ Severity::Info, ss.str() });
	}
}