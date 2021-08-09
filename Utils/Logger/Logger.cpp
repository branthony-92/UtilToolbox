#include "stdafx.h"

#include "Logger.h"
#include "LogAction.h"

using namespace std::chrono_literals;

namespace {
	const std::map<Severity, std::string> c_sevReflectionMap = {
		{ Severity::Info,	  " [INFO] "  },
		{ Severity::Debug,    " [DEBUG] " },
		{ Severity::Warning,  " [WARN] "  },
		{ Severity::ErrorL1,  " [ERR1] "  },
		{ Severity::ErrorL2,  " [ERR2] "  },
		{ Severity::Fatal,    " [FATAL] " }
	};
}

namespace Constants {
	const unsigned int c_logWriteChunkSize = 10;
}

CLogger::CLogger(std::string logPath)
	: CActionContext("Logger")
	, m_logName("")
	, m_logPath(logPath)
	, m_logs()
	, m_logLevel(Severity::Normal)
    , m_logThread()
    , m_writeEnabled(false)
    , m_mutex()
	, m_pLogAction(nullptr)
	, m_writeMode(WriteMode::NO_WRITE)
{
	m_pLogAction = std::make_unique<CLogAction>();
}

CLogger::~CLogger()
{
	writeChunk();
	auto pPromise = m_pLogAction->getPromise();
	if (pPromise) pPromise->waitForResult();
}

std::string CLogger::logMsg(CLogMessage log)
{
	if (!m_writeEnabled.load() || log.getSev() > m_logLevel)
	{
		return "";
	}

	auto logStr = constructMessageString(log);
	log.setMsg(logStr);
	addLog(log);
	auto nLogs = getLogSize();
	
	switch (getWriteMode())
	{
	case WriteMode::TRUNCATE:
		writeChunk();
		m_pLogAction->getPromise()->waitForResult();
		break;
	case WriteMode::APPEND:
		if (nLogs >= Constants::c_logWriteChunkSize) writeChunk();
		break;
	case WriteMode::NO_WRITE:
		break;
	}

	return logStr;
}

void CLogger::writeChunk()
{
	if (m_logs.empty()) return;

	auto pPromise = m_pLogAction->getPromise();
	if (pPromise) pPromise->waitForResult();
	m_pLogAction->run(this);
}

std::string CLogger::getFilePath()
{
	TLock lock(m_mutex);

	std::ostringstream oss;
	oss << m_logPath << m_logName << ".txt";

	return oss.str();
}

TLogQueue CLogger::getLogs()
{
	TLock lock(m_mutex); 
	auto logs = m_logs;
	m_logs.clear();
	return logs; 
}

void CLogger::purgeMessages()
{
	TLock lock(m_mutex);
	m_logs.clear();
}

std::string CLogger::getTimePointString(TimePoint timePoint)
{
	// todo - determine local
	std::time_t t = std::chrono::system_clock::to_time_t(timePoint);
	std::tm now_tm = *std::gmtime(&t);

	char buff[80];
	strftime(buff, 80, "%F %T", &now_tm);
	return buff;
}

std::string CLogger::constructMessageString(CLogMessage log)
{
	std::ostringstream oss;
	oss << getTimePointString(log.getTimestamp()) << c_sevReflectionMap.at(log.getSev());
	oss << log.getMsg();
	return oss.str();
}