#ifndef LOGGERTEST_H
#define LOGGERTEST_H

#include "Logger.h"
#include <gtest\gtest.h>

class LoggerTest : public ::testing::Test
{
protected:
	void SetUp();
	void TearDown();

	TLoggerPtr m_pLogger;
};


#endif // !LOGGERTEST_H